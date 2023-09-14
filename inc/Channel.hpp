/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbily <fbily@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 17:16:21 by fbily             #+#    #+#             */
/*   Updated: 2023/09/14 18:29:18 by fbily            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "User.hpp"
#include "Server.hpp"
#include <sstream>

class Channel
{
	private:
	
		std::string			_name;
		std::string			_topic;
		std::string			_key;
		bool				_isKey;
		bool				_inviteMode;
		bool				_topicMode;
		unsigned int		_maxUsers;
		unsigned int		_nbUser;
		std::vector<User *>	_Users;
		std::vector<User *> _Operators;
		std::vector<User *> _Invited;
		std::string			_topicTime;
		std::string			_topicUser;
		std::string			_creationDate;
		Channel();
		Channel(const Channel& obj);
		Channel& operator=(const Channel& rhs);
		
	public:
	
		Channel(std::string name, std::string key, User *user);
		~Channel();

		std::string		getName() const;
		std::string 	getTopic() const;
		bool			getModeTopic() const;
		unsigned int	getMaxUsers() const;
		unsigned int	getNbUser() const;
		std::string		getTopicDate() const;
		std::string		getTopicUser() const;
		std::string		getCreationDate() const;
		std::string		getModestring() const;
		User			*returnOperator(std::string nickname);
		User			*returnUser(std::string nickname);
		
		void			set_inviteMode(User *user, int sign);
		void			set_topicMode(User *user, int sign);
		void			set_key(User *user, std::string argument, int pos_argument, int sign);
		void			set_op(User *user, std::string argument, int pos_argument, int sign);
		void			set_maxUsers(User *user, std::string argument, int pos_argument, int sign);
		void			setTopic(User *user, std::string newTopic);
		void			setNbUser(int nb);
		
		std::string		joinChannel(std::string key, User *user);
		void			kickUser(User *user, std::string target, std::string comment);
		void			modeChannel(User *user, std::string modestring, std::string argument);
		void			sendMessage(User *user, std::string message);
		void			whoList(User *user);

		bool 			foundInvited(std::string nickname);
		bool 			foundUser(std::string nickname);
		bool 			foundOperator(std::string nickname);
		void			listUsersOnChannel(User *user);
		void			addGuest(User *user);
		bool			hasDuplicates(const std::string& str);
		void			kickModeUser(std::string target);
		void			kickModeOperator(std::string target);
		void			kickModeInvited(std::string target);
};

#endif
