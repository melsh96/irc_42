/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: c2h6 <c2h6@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 14:10:46 by meshahrv          #+#    #+#             */
/*   Updated: 2023/07/04 15:05:59 by c2h6             ###   ########.fr       */
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
					break ;
				}
				else if (it->revents & POLLHUP)
				{
					std::cerr << vert "Error: Client disconnected" fin << std::endl;
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
			this->_deleteUser(it); // le pb arrive ici car on demande la suppression d'un user qui 
		}
		else
			_chooseCmd(user);
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
	std::cout << YELLOW("[ MESSAGE RECEIVED : ") << str << YELLOW("]") << std::endl;
	user->setMessage(str); // did not set the message in the user

	return (nbyte);
}

void Server::_deleteUser(pollfd_it &it)
{

	try
	{
		std::cout << vert "deleteUser: it->fd: " << it->fd << fin << std::endl;
		User *user = this->_user.at(it->fd);
		int fd = user->getUserFd();

		if (fd != -1)
			close(fd);
		this->_pollFd.erase(it);
		this->_user.erase(fd);
		delete user;
		std::cerr << RED("Disconnecting user on socket: ") << fd << std::endl;
	}
	catch (const std::out_of_range &e)
	{
		std::cout << vert "out_of_range from deleteUser" fin << std::endl;
	}
}

void Server::_indexingCmd()
{
	_indexCmd.insert(std::pair<std::string, func>("CAP", &Server::_caplsCmd));
	_indexCmd.insert(std::pair<std::string, func>("PASS", &Server::_passCmd));
	_indexCmd.insert(std::pair<std::string, func>("NICK", &Server::_nickCmd));
	_indexCmd.insert(std::pair<std::string, func>("USER", &Server::_userCmd));
	_indexCmd.insert(std::pair<std::string, func>("QUIT", &Server::_quitCmd));
	_indexCmd.insert(std::pair<std::string, func>("PING", &Server::_pingCmd));
}

void Server::_chooseCmd(User *user)
{
	std::string msg = user->getMessage();
	std::string cmd;
	std::string buf;

	// std::cout << WHITE("CHOOSE CMD!") << std::endl;

	while (msg.length())
	{
		if (msg.find("\r\n") != std::string::npos)
		{
			if (msg.find(' ') != std::string::npos)
				cmd = msg.substr(0, msg.find(' '));
			else
				cmd = msg.substr(0, msg.find("\r\n"));
			if (cmd.length() == msg.find("\r\n"))
				buf.clear();
			else
				buf = msg.substr(cmd.length() + 1, msg.find("\r\n") - cmd.length() - 1);
			try
			{
				if (buf.find_first_not_of(' ') != std::string::npos)
					buf = buf.substr(buf.find_first_not_of(' '));
				else
					buf.clear();
				if (!buf.empty())
					buf = buf.substr(0, buf.find_last_not_of(' ') + 1);
				std::cout << "Command: <" << cmd << ">" << std::endl;
				std::cout << "Buffer: <" << buf << ">" << std::endl;
				if (cmd != "CAP" && cmd != "PASS" && !user->getPassword())
				{
					_closeConnection(user);
					break;
				}
				for (std::map<std::string, func>::iterator it = this->_indexCmd.begin(); it != this->_indexCmd.end(); it++)
				{
					if (it->first == cmd)
					{
						(this->*(it->second))(user, buf);
						break;
					}
				}
				msg.erase(0, msg.find("\r\n") + 2);
			}
			catch (const std::out_of_range &e)
			{
				msg.erase(0, msg.find("\r\n") + 2);
				user->sendReply("421 Unknown command");
			}
		}
		else
		{
			msg.clear();
			user->sendReply("Error: no \\r\\n found");
		}
	}
}

void Server::_closeConnection(User *user)
{
	try
	{
		user->sendReply("Please, enter PASS first. Disconnecting.");
		int fd = user->getUserFd();
		close(fd);
		for (pollfd_it it = this->_pollFd.begin(); it != this->_pollFd.end(); ++it)
		{
			if (fd == it->fd)
			{
				_pollFd.erase(it);
				break;
			}
		}
		_user.erase(fd);
		delete user;
		std::cout << "Disconnecting user on socket " << fd << std::endl;
	}
	catch (const std::out_of_range &e)
	{
	}
}

void Server::_caplsCmd(User *user, std::string param)
{
	std::cout << "CAP LS" << std::endl;
	if (param != "LS")
		return (user->sendReply("CAP LS command"));
}

void Server::_passCmd(User *user, std::string param)
{
	if (user->hasBeenWelcomed())
		return (user->sendReply("Error: pass: already welcomed"));
	if (!param.length())
		return (user->sendReply("Error: pass: empty"));
	if (param.compare(_password))
	{
		user->setPassword(false);
		return (user->sendReply(ERR_PASSWDMISMATCH(user->getNickname())));
	}
	user->setPassword(true);
}

void Server::_nickCmd(User *user, std::string param)
{
	if (param.empty())
		return (user->sendReply(ERR_NONICKNAMEGIVEN(param)));
	// if (!valid_user_name(param))
	// 	return (user->sendReply(ERR_NONICKNAMEGIVEN(param)));
	if (param.find(' ') != std::string::npos)
		param = param.substr(0, param.find_first_of(' '));
	for (users_iterator it = _user.begin(); it != _user.end(); ++it)
	{
		if (it->second->getNickname() == param)
			return (user->sendReply(ERR_NICKCOLLISION()));
	}
	user->setNickname(param);
}

void Server::_userCmd(User *user, std::string param)
{
	if (user->hasBeenWelcomed())
		return (user->sendReply(ERR_ALREADYREGISTRED(user->getNickname())));
	if (param.empty())
		return (user->sendReply(ERR_NEEDMOREPARAMS(user->getNickname(), "")));
	std::string username = param.substr(0, param.find(' '));
	std::string mode = param.substr(param.find(' ') + 1, param.find(' ', param.find(' ') + 1) - param.find(' ') - 1);
	std::string unused = param.substr(param.find(' ', param.find(' ') + 1) + 1, param.find(' ', param.find(' ', param.find(' ') + 1) + 1) - param.find(' ', param.find(' ') + 1) - 1);
	std::string realname;
	if (param.find(':', param.find(' ', param.find(' ') + 1) + 1) == std::string::npos)
		realname = "";
	else
		realname = param.substr(param.find(':', param.find(' ', param.find(' ') + 1) + 1) + 1);
	if (realname == "" || mode == "" || username == "")
		return (user->sendReply(ERR_NEEDMOREPARAMS(user->getNickname(), param)));
	user->setRealname(realname);
	user->setUsername(mode);
	user->setHostname(unused);
	if (user->getNickname().size() && user->getPassword() && !user->hasBeenWelcomed())
		user->welcome();
}

void	Server::_pingCmd(User *user, std::string param){
	if (param.empty())
		return (user->sendReply(ERR_NEEDMOREPARAMS(user->getNickname(), param)));
	if (param != "localhost" && param != "IRC") // change localhost to _hostname variable
		return (user->sendReply(ERR_NOSUCHSERVER(user->getNickname(), param)));
	user->sendReply(RPL_PONG(user->getNickname(), param));
}

//added this function to quit the server when the user types /quit
void	Server::_quitCmd(User *user, std::string param)
{
	if (param.empty())
		param = "No reason given by user";
	
	for (users_iterator it = _user.begin(); it != _user.end(); ++it)
	{
		if (it->second->getNickname() != user->getNickname())
			it->second->sendReply(":" + user->getNickname() + "!d@" + "localhost" + " QUIT :Quit: " + param + ";\n" + user->getNickname() + " is exiting the network with the message: \"Quit: " + param + "\"");
	}
	try
	{
		int	fd = user->getUserFd();
		close(fd);
		for (pollfd_it it = _pollFd.begin(); it != _pollFd.end(); ++it)
		{
			if (fd == it->fd)
			{
				std::cout << rouge "ERASING user on socket " << fd << fin << std::endl;
				_pollFd.erase(it);
				break;
			}
		}
		_user.erase(fd);
		std::cout << rouge "ERASING user with socket " << fd << fin << std::endl;

		delete user;
		std::cout << rouge "Disconnecting user on socket " << fd << fin << std::endl;
	}
	catch (const std::out_of_range &e) {
		std::cout << vert "ERROR c'est ici que je rentre" fin << std::endl;
	}
}