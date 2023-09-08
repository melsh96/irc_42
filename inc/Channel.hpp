/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhamdouc <zhamdouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 17:16:21 by fbily             #+#    #+#             */
/*   Updated: 2023/09/08 17:55:54 by zhamdouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "User.hpp"
#include "Server.hpp"

class Channel
{
	private:
	
		std::string			_name;
		std::string			_topic;
		std::string			_key;
		bool				_inviteMode;
		// bool				_topicMode;
		// bool				_opMode;
		unsigned int		_maxUsers;
		unsigned int		_nbUser;
		std::vector<User *>	_Users;
		std::vector<User *> _Operators;
		std::vector<User *> _Invited;
		Channel();
		Channel(const Channel& obj);
		Channel& operator=(const Channel& rhs);
		
	public:
	
		Channel(std::string name, std::string key, User *user);
		~Channel();

		std::string		getName() const;
		std::string 	getTopic() const;
		
		std::string		joinChannel(std::string key, User *user);
		bool 			foundInvited(std::string nickname);
		bool 			foundUser(std::string nickname);
		bool 			foundOperator(std::string nickname);
		void			addGuest(User *user);
		void			sendMessage(User *user, std::string message);

		//ajouter avant a la liste les invites 
		
};

#endif
