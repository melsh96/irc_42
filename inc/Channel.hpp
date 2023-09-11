/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbily <fbily@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 17:16:21 by fbily             #+#    #+#             */
/*   Updated: 2023/09/11 19:34:53 by fbily            ###   ########.fr       */
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
		bool				_inviteMode;
		bool				_topicMode;//si c'est vrai seul les op peuvent le modifier
		unsigned int		_maxUsers;
		unsigned int		_nbUser;
		std::vector<User *>	_Users;
		std::vector<User *> _Operators;
		std::vector<User *> _Invited;
		std::string			_topicTime;
		std::string			_topicUser;
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
		std::string		getTopicDate() const;
		std::string		getTopicUser() const;
		User			*returnOperator(std::string nickname); // ajouter const ?
		User			*returnUser(std::string nickname); // ajouter const ?
		
		void			set_inviteMode(std::string modestring);
		void			set_topicMode(std::string modestring);
		void			set_key(std::string argument, int pos_argument, std::string modestring);
		void			set_op(User *user, std::string argument, int pos_argument, std::string modestring);
		void			set_maxUsers(std::string argument, int pos_argument, std::string modestring);
		void			setTopic(User *user, std::string newTopic);
		
		std::string		joinChannel(std::string key, User *user);
		void			kickUser(User *user, std::string target, std::string comment);
		void			modeChannel(User *user, std::string modestring, std::string argument, std::string channel);
		void			sendMessage(User *user, std::string message);

		bool 			foundInvited(std::string nickname);
		bool 			foundUser(std::string nickname);
		bool 			foundOperator(std::string nickname);
		void			listUsersOnChannel(User *user);
		void			addGuest(User *user);
		bool			hasDuplicates(const std::string& str);
		void			kickModeUser(std::string target);
		void			kickModeOperator(std::string target);
		//ajouter avant a la liste les invites 
		
};

#endif
