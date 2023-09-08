/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhamdouc <zhamdouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 14:10:07 by meshahrv          #+#    #+#             */
/*   Updated: 2023/09/08 15:18:36 by zhamdouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include <poll.h>
#include <vector>
#include <cstdlib>
#include <map>
#include <signal.h>

#include "Color.hpp"
#include "User.hpp"
#include "Messages.hpp"
#include "Channel.hpp"

class User;
class Channel;

class Server {
	
	typedef std::vector<pollfd>::iterator	pollfd_it;
	typedef std::map<int, User *>::iterator users_iterator;
	typedef void (Server::*func)(User *, std::string);

	private:
		std::string						_port;
		std::string						_password;
		int								_listenSocket;

		std::vector<struct pollfd>   	_pollFd;
		std::map<int, User *>   		_user;
		std::map<std::string, func>   	_indexCmd;

		std::map<std::string, Channel *> _channels;

		void    						_createServer(void);
		void							_runServer();
		void							_acceptConnection();
		void							_receiveData(pollfd_it &it);
		int								_getData(User *user);
		void							_deleteUser(pollfd_it &it);
		
		void							_indexingCmd();
		void							_parseCmd(User *user);
		void							_closeConnection(User *user);

        void							_caplsCmd(User *user, std::string param);
		void							_passCmd(User *user, std::string param);
		void							_nickCmd(User *user, std::string param);
		void							_userCmd(User *user, std::string param);
		void							_pingCmd(User *user, std::string param);
		void							_quitCmd(User *user, std::string param);
		void							_joinCmd(User *user, std::string param);
		void							_inviteCmd(User *user, std::string param);
		void							_privmsgCmd(User *user, std::string param);

		void							_clean();

		
	public:
		Server();
		Server(std::string port, std::string password);
		~Server();
	
};

#endif