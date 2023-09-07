/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbily <fbily@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 17:16:21 by fbily             #+#    #+#             */
/*   Updated: 2023/09/07 17:31:22 by fbily            ###   ########.fr       */
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
		Channel();
		Channel(const Channel& obj);
		Channel& operator=(const Channel& rhs);
		
	public:
	
		Channel(std::string name, std::string key, User *user);
		~Channel();

		std::string	getName() const;
		std::string getTopic() const;
		
		void		joinChannel(std::string key, User *user);
		void		inviteChannel();
		
};

#endif
