/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zheylkoss <zheylkoss@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 17:16:21 by fbily             #+#    #+#             */
/*   Updated: 2023/09/11 01:06:45 by zheylkoss        ###   ########.fr       */
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
		bool				_topicMode;//si c'est vrai seul les op peuvent le modifier
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
		void			kickUser(User *user, std::string target, std::string comment);
		void			listUsersOnChannel(User *user);
		void			modeChannel(User *user, std::string modestring, std::string argument, std::string channel);
		bool			hasDuplicates(const std::string& str);
		void			set_inviteMode(std::string modestring);
		void			set_topicMode(std::string modestring);
		void			set_key(std::string argument, int pos_argument, std::string modestring);
		void			set_op(User *user, std::string argument, int pos_argument, std::string modestring);
		void			kickModeOperator(std::string target);
		User			*returnOperator(std::string nickname);
		User			*returnUser(std::string nickname);
		void			kickModeUser(std::string target);
		void			set_maxUsers(std::string argument, int pos_argument, std::string modestring);
		unsigned int	get_maxUsers(void);
		//ajouter avant a la liste les invites 
		
};

#endif
