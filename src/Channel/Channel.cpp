/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbily <fbily@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 17:28:51 by fbily             #+#    #+#             */
/*   Updated: 2023/09/07 17:31:43 by fbily            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

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

void	Channel::joinChannel(std::string key, User *user)
{
	if ()
	if (key == this->_key) // Check aussi limits user ?
	{
		this->_Users.push_back(user);
		this->_nbUser++;
	}
}
