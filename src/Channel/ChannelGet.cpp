/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelGet.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhamdouc <zhamdouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 14:29:56 by zhamdouc          #+#    #+#             */
/*   Updated: 2023/09/15 14:41:55 by zhamdouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

User* Channel::returnOperator(std::string nickname)
{
	std::vector<User *>::iterator it = this->_Operators.begin();
	while (it != this->_Operators.end())
	{
		if (nickname != (*it)->getNickname())
			it++;
		else
			return (*it);
	}
	return (*it);
}

User* Channel::returnUser(std::string nickname)
{
	std::vector<User *>::iterator it = this->_Users.begin();
	while (it != this->_Users.end())
	{
		if (nickname != (*it)->getNickname())
			it++;
		else
			return (*it);
	}
	return (*it);
}

unsigned int Channel::getNbUser() const
{
	return (this->_nbUser);
}

unsigned int	Channel::getMaxUsers() const
{
	return (this->_maxUsers);
}

std::string	Channel::getName() const
{
	return (this->_name);
}

std::string Channel::getTopic() const
{
	return (this->_topic);
}

bool Channel::getModeTopic() const
{
	return (this->_topicMode);
}

std::string	Channel::getTopicDate() const
{
	return (this->_topicTime);
}

std::string	Channel::getTopicUser() const
{
	return (this->_topicUser);
}

std::string Channel::getCreationDate() const
{
	return (this->_creationDate);
}

std::string Channel::getModestring() const
{
	std::string	modestring = "+";

	if (this->_inviteMode)
		modestring.append("i");
	if (this->_topicMode)
		modestring.append("t");
	if (this->_isKey == true)
		modestring.append("k");
	if (this->_maxUsers != 0)
	{
		std::ostringstream oss;
		oss << this->_maxUsers;
		modestring.append("l ");
		modestring.append(oss.str());
	}
	if (modestring == "+")
		modestring.clear();
	return (modestring);
}
