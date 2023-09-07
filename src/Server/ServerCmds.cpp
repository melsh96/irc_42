/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCmds.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhamdouc <zhamdouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 16:23:13 by meshahrv          #+#    #+#             */
/*   Updated: 2023/09/07 19:14:47 by zhamdouc         ###   ########.fr       */
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
	_indexCmd.insert(std::pair<std::string, func>("JOIN", &Server::_joinCmd));
	_indexCmd.insert(std::pair<std::string, func>("INVITE", &Server::_inviteCmd));
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
			return (user->sendReply(ERR_NICKCOLLISION(param)));
	}
	std::string oldname =user->getNickname();
	user->setNickname(param);
	//return (user->sendReply(":" + oldname + "!" + user->getUsername() + "@localhost" + " NICK " + param));
	return (user->sendReply(":" + oldname + " NICK " + param));
}

void    Server::_userCmd(User *user, std::string param)
{
	if (user->hasBeenWelcomed())
		return (user->sendReply(ERR_ALREADYREGISTERED(user->getNickname())));
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

void	Server::_joinCmd(User *user, std::string param)
{
	std::map<std::string, std::string>	channel;
	std::string chans;
	std::string keys;
	if (param.find(' ') != std::string::npos)
	{
		chans = param.substr(0, param.find(' '));
		keys = param.substr(param.find(' ') + 1);
	}
	else
		chans = param;
	size_t pos1 = 0;
	size_t pos2 = 0;
	while (!chans.empty())
	{
		if ((pos1 = chans.find(',')) != std::string::npos)
		{
			std::string token1 = chans.substr(0, pos1);
			if ((pos2 = keys.find(',')) != std::string::npos)
			{
				std::string token2 = keys.substr(0, pos2);
				channel[token1] = token2;
				keys.erase(0, pos2 + 1);
			}
			else
				channel[token1] = '\0';
			chans.erase(0, pos1 + 1);
		}
		channel[chans] = keys;
		chans.clear();
		keys.clear();
	}
	std::map<std::string, std::string>::iterator it = channel.begin();
	while (it != channel.end())
	{
		if (this->_channels.find(it->first) != this->_channels.end())
		{
			// join channel
			std::string result = this->_channels[it->first]->joinChannel(it->second, user);
			user->sendReply(result);
		}
		else
		{
			// create channel
			this->_channels[it->first] = new Channel(it->first, it->second, user);
			user->sendReply(':' + user->getNickname() + " JOIN " + it->first);
		}
		it++;
	}
}

void	Server::_inviteCmd(User *user, std::string param)
{
	std::map<std::string, std::string>	channel;
	std::string chans;
	std::string guest;
	if (param.find(' ') != std::string::npos)
	{
		guest = param.substr(0, param.find(' '));
		chans = param.substr(param.find(' ') + 1);
	}
	else
		return ;//il manque un argument batard
	// verifier que guest existe, et recuperer le pointeur pour pouvoir add
	if (this->_channels.find(chans) != this->_channels.end())
	{
		//  channel finded
		if (this->_channels[chans]->foundUser(user->getNickname()) == true)
			return ();//celui qui invite ne doit pas etre un user mais un operateur
		else if (this->_channels[chans]->foundOperator(user->getNickname()) == false)
			return ();//doit etre dans le channel 
		if (this->_channels[chans]->foundUser(guest) == true || this->_channels[chans]->foundOperator(guest) == true)
			return ();//guest est dans le channel
		if (this->_channels[chans]->foundInvited(guest) == false)
			// this->_channels[chans]->addGuest(User *user);
		//return (user->sendReply());
	}
	else
		return ;// channel non trouver
}