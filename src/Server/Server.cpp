/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meshahrv <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 14:10:46 by meshahrv          #+#    #+#             */
/*   Updated: 2023/07/04 17:43:48 by meshahrv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

// Serveur :
// 1.	Créer une adresse en mémoire (getaddrinfo)
// 2.	Créer le socket (socket)
// 3.	Relier Adress <=> Socket (bind)
// 4.	Écouter sur le canal (listen)
// 5.	Démarrer le serveur (poll)

// Apres poll :
// 1. soit accept() => accepter la connection
// 2. soit recv() => receive data inside event
// 3. soit send()

bool loop;

Server::Server()
{

	// std::cout << GREEN("Default Constructor") << std::endl;
}

Server::Server(std::string port, std::string password)
{

	this->_port = port;
	this->_password = password;

	_createServer();
	std::cout << YELLOW("[ SERVER CREATED ]") << std::endl
			  << std::endl;
	_indexingCmd();
	_runServer();
	// std::cout << YELLOW("Server Constructor") << std::endl;
}

Server::~Server()
{

	// std::cout << RED("Default Destructor") << std::endl;
}

void Server::_createServer(void)
{

	int add;
	int socketFd;
	struct addrinfo hints;
	struct addrinfo *result = NULL;

	std::memset(&hints, 0, sizeof(hints));

	/* Allow IPv4 or IPv6 */
	hints.ai_family = AF_UNSPEC;

	/* TCP (SOCK_STREAM) */
	hints.ai_socktype = SOCK_STREAM;

	/* For wildcard IP address */
	hints.ai_flags = AI_PASSIVE;

	/* 1. Allocate memory */
	add = getaddrinfo(NULL, this->_port.c_str(), &hints, &result);
	if (add != 0)
	{
		std::cerr << RED("Error: ") << "getaddrinfo(): " << gai_strerror(add) << std::endl;
		exit(1);
	}
	std::cout << GREEN("### Success ### ") << GREY("...getaddrinfo()") << std::endl;

	/* 2. Create Socket */
	socketFd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (socketFd == -1)
	{
		std::cerr << RED("Error: ") << "socket() failed" << std::endl;
		freeaddrinfo(result);
		exit(1);
	}
	std::cout << GREEN("### Success ### ") << GREY("...socket()") << std::endl;

	/* 3. Link adress + socket */
	if (bind(socketFd, result->ai_addr, result->ai_addrlen) == -1)
	{
		std::cerr << RED("Error: ") << "bind() failed" << std::endl;
		close(socketFd);
		freeaddrinfo(result);
		exit(1);
	}
	std::cout << GREEN("### Success ### ") << GREY("...bind()") << std::endl;

	/* 4. Listen on the canal */
	if (listen(socketFd, 1000) == -1)
	{
		std::cerr << RED("Error: ") << "listen() failed" << std::endl;
		close(socketFd);
		freeaddrinfo(result);
		exit(1);
	}
	std::cout << GREEN("### Success ### ") << GREY("...listen()") << std::endl;

	this->_listenSocket = socketFd;
}

void Server::_runServer()
{

	pollfd serverFd;
	int pollRes;

	/* Set up the initial listening socket */
	std::cout << "listensocket: " << this->_listenSocket << std::endl;
	serverFd.fd = this->_listenSocket;
	serverFd.events = POLLIN | POLLOUT;
	serverFd.revents = 0;

	this->_pollFd.push_back(serverFd);

	/*
		Loop waiting for incoming connections or for incoming
		data on any of the connected sockets.

		1. Is there a connection ?
	*/
	while (loop)
	{
		// std::cout << GREEN("DEBUG:") << "[INSIDE]" << std::endl;
		pollRes = poll(this->_pollFd.data(), this->_pollFd.size(), -1); /* -1 to wait indefinitely */

		if (pollRes != -1)
		{
			for (pollfd_it it = this->_pollFd.begin(); it != this->_pollFd.end(); it++)
			{
				/* Check if an event has been activated in revent */
				if (it->revents & (POLLIN | POLLOUT))
				{
					if (it->fd == this->_listenSocket)
					{
						this->_acceptConnection();
						break;
					}
					this->_receiveData(it);
					break ; // added this
				}
				else if (it->revents & POLLHUP)
				{
					std::cerr << RED("Error: ") << "Client disconnected" << std::endl;
					close(it->fd);
                    this->_pollFd.erase(it);
					break;
				}
				else if (it->revents & POLLERR)
				{
					std::cerr << RED("Error: ") << "poll() failed" << std::endl;
					close(it->fd);
                    this->_pollFd.erase(it);
					break;
				}
				else if (it == this->_pollFd.end())
					break;
			}
		}
	}
	this->_clean();
	std::cerr << std::endl << YELLOW("[ SERVER SAFELY CLOSED ]") << std::endl;
}

void Server::_acceptConnection()
{

	struct sockaddr_storage userAddr;
	socklen_t userLen = sizeof(userAddr);

	int userFd = accept(this->_listenSocket, (struct sockaddr *)&userAddr, &userLen);
	if (userFd == -1)
		std::cerr << RED("Error: ") << "accept() failed" << std::endl;
	else
	{
		pollfd userPollFd;

		userPollFd.fd = userFd;
		userPollFd.events = POLLIN;
		userPollFd.revents = 0;

		this->_pollFd.push_back(userPollFd);
		this->_user.insert(std::pair<int, User *>(userFd, new User(userFd, &userAddr)));
		std::cout << GREEN("### Success ### ") << GREY("...accept()") << std::endl;
		std::cout << YELLOW("[ USER LOGGED IN ]") << std::endl
				  << std::endl;
	}
}

void Server::_receiveData(pollfd_it &it)
{

	try
	{
		std::cout << BLUE("### RECIEVED DATA ###") << std::endl;
		User *user = this->_user.at(it->fd);
		int nbyte = 0;


		nbyte = this->_getData(user);
		std::cout << GREY("byte sent: ") << nbyte << std::endl;

		if (nbyte <= 0)
		{
			if (nbyte == 0)
				std::cerr << RED("Error: ") << "socket hung up" << std::endl;
			else
				std::cerr << RED("Error: ") << "recv()" << std::endl;
			this->_deleteUser(it);
		}
		else
			_parseCmd(user); // essentiel pour connectio du cliet
	}
	catch (const std::out_of_range &e)
	{
		std::cout << RED("Error: ") << "out_of_range" << std::endl;
	}
}

int Server::_getData(User *user)
{

	int nbyte = 0;
	int fd = user->getUserFd();
	char buffer[1024];
	std::string str;

	user->clearMessage();
	while (str.rfind("\r\n") != str.length() - 2 || str.length() < 2)
	{
		memset(buffer, 0, sizeof(buffer));
		nbyte = recv(fd, buffer, sizeof(buffer), 0);
		if (nbyte <= 0)
			break;
		str.append(buffer);
	}

	std::cout << GREEN("### Success ### ") << GREY("...recv()") << std::endl;
	std::cout << YELLOW("[ MESSAGE RECEIVED : ") << str << YELLOW("]") << std::endl << std::endl;
	user->setMessage(str); // did not set the message in the user

	return (nbyte);
}
