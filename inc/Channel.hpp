/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbily <fbily@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 17:16:21 by fbily             #+#    #+#             */
/*   Updated: 2023/09/06 18:42:14 by fbily            ###   ########.fr       */
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
		// bool				_inviteMode;
		// bool				_topicMode;
		// bool				_opMode;
		// unsigned int		_maxUsers;
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
		
};

#endif

/* 
Fonctions a faire :
	- JOIN EN PRIO
	- Invite
	- Kick
	- Topic
	- Mode :
				-i (set/unset invite only)
				-t ()
				-k (set/unset key)
				-o (Give/Take operator privilege)
				-l (set/unset user limit)
	- Map dans serveur.cpp => std::map<string, channel*> [name] = *channel;
	
A savoir :
	- Recoit on les user* a inviter ? Ex : Zak invite Flo sur channel 42 => On recoit Zak mais Flo ?
	- Variable par default pour creation channel

/!\ Savoir pour les leaks Channel en fin d'exec /!\

 */