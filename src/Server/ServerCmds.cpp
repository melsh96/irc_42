/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCmds.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meshahrv <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 16:23:13 by meshahrv          #+#    #+#             */
/*   Updated: 2023/07/04 16:33:49 by meshahrv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

void Server::_indexingCmd()
{
	_indexCmd.insert(std::pair<std::string, func>("CAP", &Server::_caplsCmd));
	_indexCmd.insert(std::pair<std::string, func>("PASS", &Server::_passCmd));
	_indexCmd.insert(std::pair<std::string, func>("NICK", &Server::_nickCmd));
	_indexCmd.insert(std::pair<std::string, func>("USER", &Server::_userCmd));
	_indexCmd.insert(std::pair<std::string, func>("QUIT", &Server::_quitCmd));
	_indexCmd.insert(std::pair<std::string, func>("PING", &Server::_pingCmd));
}

void Server::_parseCmd(User *user)
{
	std::string msg = user->getMessage();
	std::string cmd;
	std::string buf;

	// std::cout << WHITE("CHOOSE CMD!") << std::endl;

	while (msg.length())
	{
		if (msg.find("\r\n") != std::string::npos)
		{
			if (msg.find(' ') != std::string::npos)
				cmd = msg.substr(0, msg.find(' '));
			else
				cmd = msg.substr(0, msg.find("\r\n"));
			if (cmd.length() == msg.find("\r\n"))
				buf.clear();
			else
				buf = msg.substr(cmd.length() + 1, msg.find("\r\n") - cmd.length() - 1);
			try
			{
				if (buf.find_first_not_of(' ') != std::string::npos)
					buf = buf.substr(buf.find_first_not_of(' '));
				else
					buf.clear();
				if (!buf.empty())
					buf = buf.substr(0, buf.find_last_not_of(' ') + 1);
				std::cout << "Command: <" << cmd << ">" << std::endl;
				std::cout << "Buffer: <" << buf << ">" << std::endl;
				if (cmd != "CAP" && cmd != "PASS" && !user->getPassword())
				{
					_closeConnection(user);
					break;
				}
				for (std::map<std::string, func>::iterator it = this->_indexCmd.begin(); it != this->_indexCmd.end(); it++)
				{
					if (it->first == cmd)
					{
						(this->*(it->second))(user, buf);
						break;
					}
				}
				msg.erase(0, msg.find("\r\n") + 2);
			}
			catch (const std::out_of_range &e)
			{
				msg.erase(0, msg.find("\r\n") + 2);
				user->sendReply("421 Unknown command");
			}
		}
		else
		{
			msg.clear();
			user->sendReply("Error: no \\r\\n found");
		}
	}
}

void    Server::_caplsCmd(User *user, std::string param)
{
	std::cout << "CAP LS" << std::endl;
	if (param != "LS")
		return (user->sendReply("CAP LS command"));
}

void    Server::_passCmd(User *user, std::string param)
{
	if (user->hasBeenWelcomed())
		return (user->sendReply("Error: pass: already welcomed"));
	if (!param.length())
		return (user->sendReply("Error: pass: empty"));
	if (param.compare(_password))
	{
		user->setPassword(false);
		return (user->sendReply(ERR_PASSWDMISMATCH(user->getNickname())));
	}
	user->setPassword(true);
}

void    Server::_nickCmd(User *user, std::string param)
{
	if (param.empty())
		return (user->sendReply(ERR_NONICKNAMEGIVEN(param)));
	// if (!valid_user_name(param))
	// 	return (user->sendReply(ERR_NONICKNAMEGIVEN(param)));
	if (param.find(' ') != std::string::npos)
		param = param.substr(0, param.find_first_of(' '));
	for (users_iterator it = _user.begin(); it != _user.end(); ++it)
	{
		if (it->second->getNickname() == param)
			return (user->sendReply(ERR_NICKCOLLISION()));
	}
	user->setNickname(param);
}

void    Server::_userCmd(User *user, std::string param)
{
	if (user->hasBeenWelcomed())
		return (user->sendReply(ERR_ALREADYREGISTRED(user->getNickname())));
	if (param.empty())
		return (user->sendReply(ERR_NEEDMOREPARAMS(user->getNickname(), "")));
	std::string username = param.substr(0, param.find(' '));
	std::string mode = param.substr(param.find(' ') + 1, param.find(' ', param.find(' ') + 1) - param.find(' ') - 1);
	std::string unused = param.substr(param.find(' ', param.find(' ') + 1) + 1, param.find(' ', param.find(' ', param.find(' ') + 1) + 1) - param.find(' ', param.find(' ') + 1) - 1);
	std::string realname;
	if (param.find(':', param.find(' ', param.find(' ') + 1) + 1) == std::string::npos)
		realname = "";
	else
		realname = param.substr(param.find(':', param.find(' ', param.find(' ') + 1) + 1) + 1);
	if (realname == "" || mode == "" || username == "")
		return (user->sendReply(ERR_NEEDMOREPARAMS(user->getNickname(), param)));
	user->setRealname(realname);
	user->setUsername(mode);
	user->setHostname(unused);
	if (user->getNickname().size() && user->getPassword() && !user->hasBeenWelcomed())
		user->welcome();
}

void    Server::_pingCmd(User *user, std::string param){
	if (param.empty())
		return (user->sendReply(ERR_NEEDMOREPARAMS(user->getNickname(), param)));
	if (param != "localhost" && param != "IRC") // change localhost to _hostname variable
		return (user->sendReply(ERR_NOSUCHSERVER(user->getNickname(), param)));
	user->sendReply(RPL_PONG(user->getNickname(), param));
}

//added this function to quit the server when the user types /exit
void	Server::_quitCmd(User *user, std::string param)
{
	if (param.empty())
		param = "No reason given by user";
	
	for (users_iterator it = _user.begin(); it != _user.end(); ++it)
	{
		if (it->second->getNickname() != user->getNickname())
			it->second->sendReply(":" + user->getNickname() + "!d@" + "localhost" + " QUIT :Quit: " + param + ";\n" + user->getNickname() + " is exiting the network with the message: \"Quit: " + param + "\"");
	}
	try
	{
		int	fd = user->getUserFd();
		close(fd);
		for (pollfd_it it = _pollFd.begin(); it != _pollFd.end(); ++it)
		{
			if (fd == it->fd)
			{
				std::cout << rouge "ERASING user on socket " << fd << fin << std::endl;
				_pollFd.erase(it);
				break;
			}
		}
		_user.erase(fd);
		std::cout << rouge "ERASING user with socket " << fd << fin << std::endl;

		delete user;
		std::cout << rouge "Disconnecting user on socket " << fd << fin << std::endl;
	}
	catch (const std::out_of_range &e) {
		std::cout << vert "ERROR c'est ici que je rentre" fin << std::endl;
	}
}

