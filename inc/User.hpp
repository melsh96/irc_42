/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: c2h6 <c2h6@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 18:51:42 by meshahrv          #+#    #+#             */
/*   Updated: 2023/07/04 11:25:42 by c2h6             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include <poll.h>
#include <cstdlib>

#include "Color.hpp"
#include "Messages.hpp"

class User {
	
	private:
		int                     	_fd;
		bool                    	_password;
		std::string             	_message;
		bool                    	_welcomed;
		struct sockaddr_storage		*_userAddr;
		std::string                 _username;
        std::string                 _nickname;
        std::string                 _hostname;
        std::string                 _realname;
        std::string                 _server;
		
	public:
		User();
		User(int fd, struct sockaddr_storage *userAddr);
		~User();

		int 						getUserFd();
		std::string                 getUsername(void);
		std::string                 getNickname(void);
		std::string                 getHostname(void);
        std::string                 getRealname(void);
		std::string                 getMessage(void);
		bool		                getPassword(void) const;

		void                        setUsername(std::string username);
		void                        setNickname(std::string nickname);
		void                        setHostname(std::string hostname);
        void                        setRealname(std::string realname);
		void                        setMessage(std::string message);
		void                        setPassword(bool password);
		
		void                        sendReply(std::string reply);
		void                        welcome(void);
		bool                        hasBeenWelcomed(void) const;

		std::string                 timestamp(void);
		void						clearMessage(void);
};

#endif