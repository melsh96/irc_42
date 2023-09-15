/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhamdouc <zhamdouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 17:28:51 by fbily             #+#    #+#             */
/*   Updated: 2023/09/15 15:58:39 by zhamdouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Channel.hpp"

Channel::Channel() {}

Channel::Channel(const Channel& obj)
{
	(void)obj;
}

Channel::Channel(std::string name, std::string key, User *user) : _name(name), _topic(""), _key(key), _isKey(false), _inviteMode(false), _topicMode(true),
	_maxUsers(0), _nbUser(1), _topicTime(""), _topicUser("")
{
	time_t now = time(0);
	std::ostringstream oss;
	oss << now;
	this->_Operators.push_back(user);
	this->_creationDate = oss.str();
	if (key != "")
		this->_isKey = true;
	if (key == "x")
	{
		this->_key = "";
		this->_isKey = false;
	}
}

Channel::~Channel() {}

Channel& Channel::operator=(const Channel& rhs)
{
	(void)rhs;
	return (*this);
}

std::string	Channel::joinChannel(std::string key, User *user)
{
	if (_inviteMode ==  true)
	{
		if (foundInvited(user->getNickname()) == false)
			return (ERR_INVITEONLYCHAN(user->getNickname(), this->getName()));
	}
	if ((this->_maxUsers != 0 && this->_nbUser >= this->_maxUsers) || this->_nbUser >= 1000)
		return (ERR_CHANNELISFULL(user->getNickname(), this->getName()));
	if (this->_isKey == false || (this->_isKey == true && key == this->_key))
	{
		this->_Users.push_back(user);
		this->_nbUser++;
		if(foundInvited(user->getNickname()) == true)
			kickModeInvited(user->getNickname());
		return (':' + user->getNickname() + "!" + user->getNickname() + "@" + user->getHostname() + " JOIN " + this->getName());
	}
	else
		return (ERR_BADCHANNELKEY(user->getNickname(), this->getName()));
}

bool Channel::foundUser(std::string nickname)
{
	std::vector<User *>::iterator it = this->_Users.begin();
	while (it != this->_Users.end())
	{
		if (nickname != (*it)->getNickname())
			it++;
		else
			return (true);
	}
	return (false);
}

bool Channel::foundOperator(std::string nickname)
{
	std::vector<User *>::iterator it = this->_Operators.begin();
	while (it != this->_Operators.end())
	{
		if (nickname != (*it)->getNickname())
			it++;
		else
			return (true);
	}
	return(false);
}

bool Channel::foundInvited(std::string nickname)
{
	std::vector<User *>::iterator it = this->_Invited.begin();
	while (it != this->_Invited.end())
	{
		if (nickname != (*it)->getNickname())
			it++;
		else
			return (true);
	}
	return(false);
}

void Channel::addGuest(User *user)
{
	this->_Invited.push_back(user);
}

void Channel::sendMessage(User *user , std::string message)
{
	std::vector<User *>::iterator it;
	it = this->_Operators.begin();
	while (it != this->_Operators.end())
	{
		if ((*it) != user)
			(*it)->sendReply(message);
		it++;
	}
	it = this->_Users.begin();
	while (it != this->_Users.end())
	{
		if ((*it) != user)
			(*it)->sendReply(message);
		it++;
	}
}

void	Channel::kickUser(User *user, std::string target, std::string comment)
{
	std::vector<User *>::iterator	it = this->_Users.begin();
	while (it != this->_Users.end())
	{
		if ((*it)->getNickname() == target)
		{
			this->sendMessage(user, ":" + user->getNickname() + " KICK " + this->_name + ' ' + target + ' ' + comment);
			user->sendReply(":" + user->getNickname() + " KICK " + this->_name + ' ' + target + ' ' + comment);
			this->_Users.erase(it);
			this->_nbUser--;
			break ;
		}
		it++;
	}
}

void	Channel::listUsersOnChannel(User *user)
{
	std::string result;
	result.append(":" + user->getServer() + " 353 " + user->getNickname() + " = " + this->getName() + " :");
	std::vector<User *>::iterator it = this->_Operators.begin();
	while (it != this->_Operators.end())
	{
		if (it != this->_Operators.begin())
			result += ' ';
		result.append('@' + (*it)->getNickname());
		it++;
	}
	it = this->_Users.begin();
	while (it != this->_Users.end())
	{
		result.append(' ' + (*it)->getNickname());
		it++;
	}
	user->sendReply(result);
	user->sendReply(":" + user->getServer() + " 366 " + user->getNickname() + ' ' + this->getName() + " :End of /NAMES list");
}

void	Channel::whoList(User *user)
{
	std::vector<User *>::iterator it = this->_Operators.begin();
	while (it != this->_Operators.end())
	{
		user->sendReply(":" + user->getServer() + " 352 " + user->getNickname() + ' ' + this->_name + ' ' + (*it)->getUsername() + ' ' + (*it)->getHostname() + ' ' + (*it)->getServer() + ' ' + (*it)->getNickname() + " @ :0 " + (*it)->getRealname());
		it++;
	}
	it = this->_Users.begin();
	while (it != this->_Users.end())
	{
		user->sendReply(":" + user->getServer() + " 352 " + user->getNickname() + ' ' + this->_name + ' ' + (*it)->getUsername() + ' ' + (*it)->getHostname() + ' ' + (*it)->getServer() + ' ' + (*it)->getNickname() + " H :0 " + (*it)->getRealname());
		it++;
	}
}

void	Channel::kickModeOperator(std::string target)
{
	std::vector<User *>::iterator	it = this->_Operators.begin();
	while (it != this->_Operators.end())
	{
		if ((*it)->getNickname() == target)
		{
			this->_Operators.erase(it);
			break ;
		}
		it++;
	}
}

void	Channel::kickModeUser(std::string target)
{
	std::vector<User *>::iterator	it = this->_Users.begin();
	while (it != this->_Users.end())
	{
		if ((*it)->getNickname() == target)
		{
			this->_Users.erase(it);
			break ;
		}
		it++;
	}
}

void	Channel::kickModeInvited(std::string target)
{
	std::vector<User *>::iterator	it = this->_Invited.begin();
	while (it != this->_Invited.end())
	{
		if ((*it)->getNickname() == target)
		{
			this->_Invited.erase(it);
			break ;
		}
		it++;
	}
}

void	Channel::modeChannel(User *user, std::string modestring, std::string argument)
{
	int pos_argument = 0;
	int sign = 0;

	if(modestring[0] == '+' || modestring[0] == '-')
	{
		if(modestring[0] == '+')
			sign = 1;
		if(modestring[0] == '-')
			sign = -1;
		for (size_t i = 1; i < modestring.size(); i++) 
		{
			if (modestring[i] == '+')
				sign = 1;
			if (modestring[i] == '-')
				sign = -1;
			if (modestring[i] == 'i')
				set_inviteMode(user, sign);
			if (modestring[i] == 't')
				set_topicMode(user, sign);
			if (modestring[i] == 'k')
			{
				set_key(user, argument, pos_argument, sign);
				pos_argument++;
			}
			if (modestring[i] == 'o')
			{
				set_op(user, argument, pos_argument, sign);
				pos_argument++;
			}
			if (modestring[i] == 'l')
			{
				set_maxUsers(user, argument, pos_argument, sign);
				pos_argument++;
			}
		}		
	}
	else
		return ;
}
