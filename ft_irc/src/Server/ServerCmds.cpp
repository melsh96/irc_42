/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCmds.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbily <fbily@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 16:23:13 by meshahrv          #+#    #+#             */
/*   Updated: 2023/09/15 16:29:14 by fbily            ###   ########.fr       */
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
	_indexCmd.insert(std::pair<std::string, func>("PRIVMSG", &Server::_privmsgCmd));
	_indexCmd.insert(std::pair<std::string, func>("KICK", &Server::_kickCmd));
	_indexCmd.insert(std::pair<std::string, func>("MODE", &Server::_modeCmd));
	_indexCmd.insert(std::pair<std::string, func>("TOPIC", &Server::_topicCmd));
	_indexCmd.insert(std::pair<std::string, func>("WHO", &Server::_whoCmd));
	_indexCmd.insert(std::pair<std::string, func>("NOTICE", &Server::_noticeCmd));
}

void Server::_parseCmd(User *user)
{
	std::string msg = user->getMessage();
	std::string cmd;
	std::string buf;

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
	if (param.find(' ') != std::string::npos)
		param = param.substr(0, param.find_first_of(' '));
	for (users_iterator it = _user.begin(); it != _user.end(); ++it)
	{
		if (it->second->getNickname() == param)
			return (user->sendReply(ERR_NICKCOLLISION(param)));
	}
	std::string oldname =user->getNickname();
	user->setNickname(param);
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
	if (user->getNickname().size() && user->getPassword() && !user->hasBeenWelcomed())
		user->welcome(this->_creationDate);
}

void    Server::_pingCmd(User *user, std::string param){
	if (param.empty())
		return (user->sendReply(ERR_NEEDMOREPARAMS(user->getNickname(), param)));
	if (param != user->getHostname() && param != "IRC")
		return (user->sendReply(ERR_NOSUCHSERVER(user->getNickname(), param)));
	user->sendReply(RPL_PONG(user->getNickname(), param));
}

void	Server::_quitCmd(User *user, std::string param)
{
	if (param == ":leaving")
		param = "No reason given by user";
	else
		param = param.substr(1);
	for (users_iterator it = _user.begin(); it != _user.end(); ++it)
	{
		if (it->second->getNickname() != user->getNickname())
			it->second->sendReply(":" + user->getNickname() + "!" + user->getNickname() + "@" + user->getHostname() + " QUIT :" + param + ";\n" + user->getNickname() + " is exiting the network with the message: \""+ param + "\"");
	}
	_leaveChannels(user);
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
	if (param.empty())
		return (user->sendReply(ERR_NEEDMOREPARAMS(user->getNickname(), param)));
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
		while ((pos1 = chans.find(',')) != std::string::npos)
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
			if (result == (':' + user->getNickname() + "!" + user->getNickname() + "@" + user->getHostname() + " JOIN " + it->first))
			{
				this->_channels[it->first]->sendMessage(user, result);
				if (this->_channels[it->first]->getTopic() != "")
					user->sendReply(":" + user->getServer() + " 332 " + user->getNickname() + ' ' + it->first + " :" + this->_channels[it->first]->getTopic());
				else
					user->sendReply(RPL_NOTOPIC(user->getNickname(), user->getServer(), it->first));
				this->_channels[it->first]->listUsersOnChannel(user);
			}
		}
		else
		{
			// create channel
			this->_channels[it->first] = new Channel(it->first, it->second, user);
			user->sendReply(':' + user->getNickname() + "!" + user->getNickname() + "@" + user->getHostname() + " JOIN " + it->first);
			user->sendReply(RPL_NOTOPIC(user->getNickname(), user->getServer(), it->first));
			this->_channels[it->first]->listUsersOnChannel(user);
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
		return (user->sendReply(ERR_NEEDMOREPARAMS(user->getNickname(), param)));
	std::map<int, User *>::iterator it;
	for(it = this->_user.begin(); it != this->_user.end(); it++)
	{
		if(it->second->getNickname() == guest)
			break;
	}
	if (it == this->_user.end())
		return (user->sendReply(ERR_NOSUCHNICK(user->getNickname(), guest)));
	if (this->_channels.find(chans) != this->_channels.end())
	{
		if (this->_channels[chans]->foundUser(user->getNickname()) == true)
			return (user->sendReply(ERR_CHANOPRIVSNEEDED(user->getNickname(), this->_channels[chans]->getName())));
		else if (this->_channels[chans]->foundOperator(user->getNickname()) == false)
			return (user->sendReply(ERR_NOTONCHANNEL(user->getNickname(), this->_channels[chans]->getName())));
		if (this->_channels[chans]->foundUser(it->second->getNickname()) == true || this->_channels[chans]->foundOperator(it->second->getNickname()) == true)
			return (user->sendReply(ERR_USERONCHANNEL(user->getNickname(), this->_channels[chans]->getName(), it->second->getNickname())));
		if (this->_channels[chans]->foundInvited(it->second->getNickname()) == false)
			this->_channels[chans]->addGuest(it->second);
		User * Guest = this->_findUser(guest);
		Guest->sendReply(":" + user->getNickname() + "!d@" + user->getHostname() + " INVITE " + guest + ' ' + chans);
		return (user->sendReply(RPL_INVITING(user->getNickname(), user->getServer(), this->_channels[chans]->getName(), it->second->getNickname())));
	}
	else
		return (user->sendReply(ERR_NOSUCHCHANNEL(user->getNickname(), chans)));
}

void	Server::_privmsgCmd(User *user, std::string param)
{
	std::map<std::string, std::string>	channel;
	std::string target;
	std::string text;
	if (param.empty())
		return (user->sendReply(ERR_NEEDMOREPARAMS(user->getNickname(), param)));
	target = param.substr(0, param.find(' '));
	text = param.substr(param.find(' ') + 1);
	if (target[0] != '#')
	{
		std::map<int, User *>::iterator it;
		for(it = this->_user.begin(); it != this->_user.end(); it++)
		{
			if(it->second->getNickname() == target)
				break;
		}
		if (it == this->_user.end())
		{
			return(user->sendReply(ERR_NOSUCHNICK(user->getNickname(), target)));
		}
		return (it->second->sendReply(':' + user->getNickname() + "!" + user->getNickname() + "@" + user->getHostname() + " PRIVMSG " + target + " " + text));
	}
	else
	{
		std::map<std::string, Channel*>::iterator it;
		for(it = this->_channels.begin(); it != this->_channels.end(); it++)
		{
			if(it->second->getName() == target)
				break;
		}
		if (it == this->_channels.end())
			return(user->sendReply(ERR_NOSUCHCHANNEL(user->getNickname(), target)));
		if (it->second->foundUser(user->getNickname()) == true || it->second->foundOperator(user->getNickname()) == true)
			return (it->second->sendMessage(user, (':' + user->getNickname() + "!" + user->getNickname() + "@" + user->getHostname() + " PRIVMSG " + target + " " + text)));
		else
			return (user->sendReply(ERR_CANNOTSENDTOCHAN(user->getNickname(), it->first)));
	}
}

void	Server::_kickCmd(User *user, std::string param)
{
	std::string channel;
	std::string target;
	std::string comment;
	
	channel = param.substr(0, param.find(' '));
	target = param.substr(param.find(' ') + 1);
	comment = target.substr(target.find(' ') + 1);
	target.erase(target.find(' '), comment.length() + 1);
	if (comment.length() == 1)	
		comment = "kicked for no reason, sorry bro.";
	if (this->_channels.find(channel) == this->_channels.end())
		return (user->sendReply(ERR_NOSUCHCHANNEL(user->getNickname(), channel)));
	if (this->_channels[channel]->foundOperator(target) == true)
		return (user->sendReply(ERR_UNKNOWNERROR(user->getNickname(), user->getServer(), user->getHostname(), "KICK", "Can't kick an operator.")));
	if (this->_channels[channel]->foundUser(target) == false)
		return (user->sendReply(ERR_USERNOTINCHANNEL(user->getNickname(), channel, target)));
	if (this->_channels[channel]->foundOperator(user->getNickname()) == false)
		return (user->sendReply(ERR_CHANOPRIVSNEEDED(user->getNickname(), channel)));
	this->_channels[channel]->kickUser(user, target, comment);
}

void	Server::_modeCmd(User *user, std::string param)
{
	std::string channel;
	std::string modestring;
	std::string argument;

	channel = param.substr(0, param.find(' '));
	if (param.find(' ') != std::string::npos)
		modestring = param.substr(param.find(' ') + 1);
	if (modestring.find(' ') != std::string::npos)
	{
		argument = modestring.substr(modestring.find(' ') + 1);
		modestring.erase(modestring.find(' '), argument.length() + 1);
	}
	if (channel[0] == '#')
	{
		if (this->_channels.find(channel) == this->_channels.end())
			return (user->sendReply(ERR_NOSUCHCHANNEL(user->getNickname(), channel)));
		if(this->_channels[channel]->foundUser(user->getNickname()) == false && this->_channels[channel]->foundOperator(user->getNickname()) == false)
			return (user->sendReply(ERR_NOTONCHANNEL(user->getNickname(), channel)));
		if (argument.empty() && modestring.empty())
		{
			user->sendReply(RPL_CHANNELMODEIS(user->getServer(), user->getNickname(), channel, this->_channels[channel]->getModestring()));
			return (user->sendReply(RPL_CREATIONTIME(user->getServer(), user->getNickname(), channel, this->_channels[channel]->getCreationDate())));
		}
		if (modestring == "b")
			return (user->sendReply(":" + user->getServer() + " 368 " + user->getNickname() + ' ' + channel + " :End of channel ban list"));
		if (this->_channels[channel]->foundOperator(user->getNickname()) == false)
			return (user->sendReply(ERR_CHANOPRIVSNEEDED(user->getNickname(), channel)));
		this->_channels[channel]->modeChannel(user, modestring, argument);
		user->sendReply(RPL_CHANNELMODEIS(user->getServer(), user->getNickname(), channel, this->_channels[channel]->getModestring()));
		this->_channels[channel]->sendMessage(user, RPL_CHANNELMODEIS(user->getServer(), user->getNickname(), channel, this->_channels[channel]->getModestring()));
		this->_channels[channel]->sendMessage(user, RPL_CREATIONTIME(user->getServer(), user->getNickname(), channel, this->_channels[channel]->getCreationDate()));
		return (user->sendReply(RPL_CREATIONTIME(user->getServer(), user->getNickname(), channel, this->_channels[channel]->getCreationDate())));
	}
	else
		return ;
}

void	Server::_topicCmd(User *user, std::string param)
{
	std::string channel;
	std::string topic;
	size_t pos = 0;

	pos = param.find(' ');
	channel = param.substr(0, param.find(' '));
	if(pos != std::string::npos)
		topic = param.substr(param.find(' ') + 2);
	if (this->_channels.find(channel) == this->_channels.end())
			return (user->sendReply(ERR_NOSUCHCHANNEL(user->getNickname(), channel)));
	if (this->_channels[channel]->foundUser(user->getNickname()) == false && this->_channels[channel]->foundOperator(user->getNickname()) == false)
		return (user->sendReply(ERR_NOTONCHANNEL(user->getNickname(), channel)));
	if (topic.empty())
	{
		topic = this->_channels[channel]->getTopic();
		if (topic.empty())
			return (user->sendReply(RPL_NOTOPIC(user->getNickname(), user->getServer(), channel)));
		else
		{
			user->sendReply(RPL_TOPIC(user->getNickname(), user->getServer(), channel, this->_channels[channel]->getTopic()));
			return (user->sendReply(RPL_TOPICWHOTIME(user->getNickname(), user->getServer(), channel, this->_channels[channel]->getTopicUser(), this->_channels[channel]->getTopicDate())));
		}
	}
	if(this->_channels[channel]->getModeTopic())
	{
		if (!this->_channels[channel]->foundOperator(user->getNickname()) == true)
			return (user->sendReply(ERR_CHANOPRIVSNEEDED(user->getNickname(),channel)));
		this->_channels[channel]->setTopic(user, topic);
		user->sendReply(RPL_TOPIC(user->getNickname(), user->getServer(), channel, this->_channels[channel]->getTopic()));
		return (user->sendReply(RPL_TOPICWHOTIME(user->getNickname(), user->getServer(), channel, this->_channels[channel]->getTopicUser(), this->_channels[channel]->getTopicDate())));
	}
	else
	{
		this->_channels[channel]->setTopic(user, topic);
		user->sendReply(RPL_TOPIC(user->getNickname(), user->getServer(), channel, this->_channels[channel]->getTopic()));
		return (user->sendReply(RPL_TOPICWHOTIME(user->getNickname(), user->getServer(), channel, this->_channels[channel]->getTopicUser(), this->_channels[channel]->getTopicDate())));
	}
}

void	Server::_whoCmd(User *user, std::string param)
{
	if (param[0] != '#')
		return ;
	std::map<std::string, Channel*>::iterator it;
	it = this->_channels.find(param);
	if (it == this->_channels.end())
		return (user->sendReply(ERR_NOSUCHCHANNEL(user->getNickname(), param)));
	it->second->whoList(user);
	return (user->sendReply(":" + user->getServer() + " 315 "+ user->getNickname() + ' ' + param + " :End of WHO list"));
}

void	Server::_noticeCmd(User *user, std::string param)
{
	std::map<std::string, std::string>	channel;
	std::string target;
	std::string text;
	if (param.empty())
		return (user->sendReply(ERR_NEEDMOREPARAMS(user->getNickname(), param)));
	target = param.substr(0, param.find(' '));
	text = param.substr(param.find(' ') + 1);
	if (target[0] != '#')
	{
		std::map<int, User *>::iterator it;
		for(it = this->_user.begin(); it != this->_user.end(); it++)
		{
			if(it->second->getNickname() == target)
				break;
		}
		if (it == this->_user.end())
		{
			return(user->sendReply(ERR_NOSUCHNICK(user->getNickname(), target)));
		}
		return (it->second->sendReply(':' + user->getNickname() + "!" + user->getNickname() + "@" + user->getHostname() + " NOTICE " + target + " " + text));
	}
	else
	{
		std::map<std::string, Channel*>::iterator it;
		for(it = this->_channels.begin(); it != this->_channels.end(); it++)
		{
			if(it->second->getName() == target)
				break;
		}
		if (it == this->_channels.end())
			return(user->sendReply(ERR_NOSUCHCHANNEL(user->getNickname(), target)));
		if (it->second->foundUser(user->getNickname()) == true || it->second->foundOperator(user->getNickname()) == true)
			return (it->second->sendMessage(user, (':' + user->getNickname() + "!" + user->getNickname() + "@" + user->getHostname() + " NOTICE " + target + " " + text)));
		else
			return (user->sendReply(ERR_CANNOTSENDTOCHAN(user->getNickname(), it->first)));
	}
}