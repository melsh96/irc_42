/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhamdouc <zhamdouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 17:28:51 by fbily             #+#    #+#             */
/*   Updated: 2023/09/08 12:26:00 by zhamdouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Channel.hpp"

/******		Constructors	******/

Channel::Channel() {}

Channel::Channel(const Channel& obj)
{
	(void)obj;
}

Channel::Channel(std::string name, std::string key, User *user) : _name(name), _key(key), _inviteMode(false), _maxUsers(0), _nbUser(1)
{
	this->_Operators.push_back(user);
	//Set les autres variable : voir doc pour cas par default
}

/******		Destructor	******/

Channel::~Channel() {}

/******		Operator	******/

Channel& Channel::operator=(const Channel& rhs)
{
	(void)rhs;
	return (*this);
}

/******		Member functions	******/

std::string	Channel::getName() const
{
	return (this->_name);
}

std::string Channel::getTopic() const
{
	return (this->_topic);
}

std::string	Channel::joinChannel(std::string key, User *user)
{
	if (_inviteMode ==  true)
	{
		if (foundInvited(user->getNickname()) == false)
			return (ERR_INVITEONLYCHAN(user->getNickname(), this->getName()));//il est pas invite ce batard  
	}
	if (this->_nbUser >= this->_maxUsers)
		return (ERR_CHANNELISFULL(user->getNickname(), this->getName()));//erreur plus de place
	
	if (key == this->_key)
	{
		this->_Users.push_back(user);
		this->_nbUser++;
		return (':' + user->getNickname() + " JOIN " + this->getName());
	}
	else
		return (ERR_BADCHANNELKEY(user->getNickname(), this->getName()));//erreur de mot de passe 
}

bool Channel::foundUser(std::string nickname)
{
	std::vector<User *>::iterator it = this->_Users.begin();
	while (it != this->_Users.end())
	{
		if (nickname != (*it)->getNickname())
			it++;
		else
			return (true);//trouve 
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