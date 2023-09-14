/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meshahrv <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 18:45:15 by meshahrv          #+#    #+#             */
/*   Updated: 2023/09/14 12:42:47 by meshahrv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/User.hpp"
#include <unistd.h>

User::User() {

	// std::cout << GREEN("Default Constructor") << std::endl;

}

// une des erreurs etait lié aux variables non initialisées et essentiel pour la suite.
User::User(int fd, struct sockaddr_storage *userAddr) : _password(false), _welcomed(false), _server("IRC") {

	char hostname[256];

	gethostname(hostname, sizeof(hostname));
	this->_hostname = hostname;
    this->_fd = fd;
    this->_userAddr = userAddr;
    
}

User::~User() {
	
	// std::cout << RED("Default Destructor") << std::endl;
}

int User::getUserFd() {
    
    return this->_fd;
}

std::string	User::getServer(){
	return(_server);
}

std::string	User::getUsername(){
	return(_username);
}

std::string	User::getNickname(){
	return(_nickname);
}

std::string    User::getHostname(){
	return(_hostname);
}

std::string    User::getRealname(){
	return(_realname);
}

std::string    User::getMessage(){
	return(_message);
}

bool	User::getPassword(void) const {
	return (_password);
}

void	User::setUsername(std::string username){
	_username = username;
}

void	User::setNickname(std::string nickname){
	_nickname = nickname;
}

void    User::setHostname(std::string hostname){
	_hostname = hostname;
}

void    User::setRealname(std::string realname){
	_realname = realname;
}

void    User::setMessage(std::string message){
	_message = message;
}

void    User::setPassword(bool password) {
	_password = password;
}

void	User::welcome(std::string date)
{
	sendReply(RPL_WELCOME(_nickname, _server, _username, _hostname));
	sendReply(RPL_YOURHOST(_nickname, _server));
	sendReply(RPL_CREATED(_nickname, _server, date));
	sendReply(RPL_MYINFO(_nickname, _server));
	_welcomed = true;
}

bool	User::hasBeenWelcomed() const{
	return (_welcomed);
}

void	User::clearMessage(){
	_message.clear();
}

void	User::sendReply(std::string reply)
{
	reply.append("\r\n");
	size_t	total = 0;
	size_t	nbytes = reply.length();
	int		n;

	std::cout << lila << (getNickname() + " SEND =>" + reply) << fin << std::endl;
	while (total < nbytes)
	{
		n = send(_fd, &(reply[total]), nbytes - total, 0);
		if (n == -1) break;
		total += n;
	}
	if (n == -1)
	{
		std::cerr << "Error User::sendReply" << std::endl;
	}
}

// std::string User::timestamp()
// {
// 	time_t now = time(0);
// 	struct tm tstruct;
// 	char buf[80];

// 	tstruct = *localtime(&now);
// 	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
	
// 	return (buf);
// }