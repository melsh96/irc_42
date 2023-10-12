/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelSet.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbily <fbily@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 14:30:40 by zhamdouc          #+#    #+#             */
/*   Updated: 2023/09/15 16:49:17 by fbily            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

void Channel::setNbUser(int nb)
{
	this->_nbUser += nb;
}

void Channel::setTopic(User *user, std::string newTopic)
{
	time_t	now = time(0);
	std::ostringstream oss;
	oss << now;
	if(newTopic == ":")
		this->_topic = "";
	else
		this->_topic = newTopic;
	this->_topicTime = oss.str();
	this->_topicUser = user->getNickname();
	sendMessage(user, RPL_TOPIC(user->getNickname(), user->getServer(), this->_name, this->_topic));
	sendMessage(user, RPL_TOPICWHOTIME(user->getNickname(), user->getServer(), this->_name, this->_topicUser, this->_topicTime));
}

void	Channel::set_inviteMode(User *user, int sign)
{
	if (sign == 1)
	{
		this->_inviteMode = true;
		user->sendReply(":" + user->getNickname() + " MODE " + this->_name + " +i");
		sendMessage(user, ":" + user->getNickname() + " MODE " + this->_name + " +i");
	}
	if (sign == -1)
	{
		this->_inviteMode = false;
		user->sendReply(":" + user->getNickname() + " MODE " + this->_name + " -i");
		sendMessage(user, ":" + user->getNickname() + " MODE " + this->_name + " -i");
	}
}

void	Channel::set_topicMode(User *user, int sign)
{
	if (sign == 1)
	{
		this->_topicMode = true;
		user->sendReply(":" + user->getNickname() + " MODE " + this->_name + " +t");
		sendMessage(user, ":" + user->getNickname() + " MODE " + this->_name + " +t");
	}
	if (sign == -1)
	{
		this->_topicMode = false;
		user->sendReply(":" + user->getNickname() + " MODE " + this->_name + " -t");
		sendMessage(user, ":" + user->getNickname() + " MODE " + this->_name + " -t");
	}
}

void	Channel::set_key(User *user, std::string argument, int pos_argument, int sign)
{
	if (sign == -1)
	{
		this->_isKey = false;
		this->_key = "";
		user->sendReply(":" + user->getNickname() + " MODE " + this->_name + " -k");
		sendMessage(user, ":" + user->getNickname() + " MODE " + this->_name + " -k");
		return ;
	}
	std::string arg_1;
	int pos_bis = 0;
	size_t pos;
	
	while (!argument.empty() && pos_bis <= pos_argument)
	{
		pos = argument.find(' ');
		arg_1 = argument.substr(0, argument.find(' '));
		if (pos_bis == pos_argument)
			break;
		if (pos == std::string::npos)
		{
			argument.erase(0, arg_1.length());  
			break;
		}
		else
			argument.erase(0, arg_1.length() + 1);
		pos_bis++;
	}
	if (!argument.empty())
	{
		if (arg_1 == "x")
			return (user->sendReply(":" + user->getServer() + " 525 " + user->getNickname() + ' ' + this->_name + " :Key is not well-formed (x is not a valid key)"));
		this->_isKey = true;
		this->_key = arg_1;
		user->sendReply(":" + user->getNickname() + " MODE " + this->_name + " +k");
		sendMessage(user, ":" + user->getNickname() + " MODE " + this->_name + " +k");
	}
}

void	Channel::set_op(User *user, std::string argument, int pos_argument, int sign)
{
	std::string arg_1;
	int pos_bis = 0;
	size_t pos = 0;

	if (argument.empty())
		return ;
	while (!argument.empty() && pos_bis <= pos_argument)
	{
		pos = argument.find(' ');
		arg_1 = argument.substr(0, argument.find(' '));
		if (pos_bis == pos_argument)
			break;
		if (pos == std::string::npos)
		{
			argument.erase(0, arg_1.length());  
			break;
		}
		else
			argument.erase(0, arg_1.length() + 1);
		pos_bis++;
	}
	if (foundOperator(user->getNickname()))
	{
		if (sign == -1 && foundOperator(arg_1) == true)
		{
			this->_Users.push_back(returnOperator(arg_1));
			kickModeOperator(arg_1);
			user->sendReply(":" + user->getNickname() + " MODE " + this->_name + " -o " + arg_1);
			sendMessage(user, ":" + user->getNickname() + " MODE " + this->_name + " -o " + arg_1);
		}
		if (sign == 1 && foundOperator(arg_1) == false)
		{
			if (foundUser(arg_1) == false)
				return (user->sendReply(ERR_USERNOTINCHANNEL(user->getNickname(), this->_name, arg_1)));
			this->_Operators.push_back(returnUser(arg_1));
			kickModeUser(arg_1);
			user->sendReply(":" + user->getNickname() + " MODE " + this->_name + " +o " + arg_1);
			sendMessage(user, ":" + user->getNickname() + " MODE " + this->_name + " +o " + arg_1);
		}
	}
	else
		return ;
}

void	Channel::set_maxUsers(User *user, std::string argument, int pos_argument, int sign)
{
	if (sign == -1)
	{
		this->_maxUsers = 0;
		user->sendReply(":" + user->getNickname() + " MODE " + this->_name + " -l");
		sendMessage(user, ":" + user->getNickname() + " MODE " + this->_name + " -l");
		return ;
	}
	std::string arg_1;
	size_t pos = 0;
	int pos_bis = 0;
	while (!argument.empty() && pos_bis <= pos_argument)
	{
		pos = argument.find(' ');
		arg_1 = argument.substr(0, argument.find(' '));
		if (pos_bis == pos_argument)
			break;
		if (pos == std::string::npos)
		{
			argument.erase(0, arg_1.length());  
			break;
		}
		else
			argument.erase(0, arg_1.length() + 1);
		pos_bis++;
	}
	pos = arg_1.find_first_not_of("1234567890");
	if (pos != std::string::npos)
		return (user->sendReply(ERR_UNKNOWNERROR(user->getNickname(), user->getServer(), user->getHostname(), "MODE", "only digit for the limit")));
	if (arg_1.length() > 3)
		return (user->sendReply(ERR_UNKNOWNERROR(user->getNickname(), user->getServer(), user->getHostname(), "MODE", "limit max of user in a channel is 999")));
	if (!argument.empty() && std::atoi(arg_1.c_str()) >= static_cast<int>(this->_nbUser))
	{
		this->_maxUsers = std::atoi(arg_1.c_str());
		user->sendReply(":" + user->getNickname() + " MODE " + this->_name + " +l " + arg_1);
		sendMessage(user, ":" + user->getNickname() + " MODE " + this->_name + " +l " + arg_1);
	}
}

