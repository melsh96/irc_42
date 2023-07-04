/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meshahrv <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 18:45:15 by meshahrv          #+#    #+#             */
/*   Updated: 2023/07/04 11:04:50 by meshahrv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/User.hpp"

User::User() {

	// std::cout << GREEN("Default Constructor") << std::endl;

}

User::User(int fd, struct sockaddr_storage *userAddr) {

    this->_fd = fd;
    this->_userAddr = userAddr;
    
}


User::~User() {
	
	// std::cout << RED("Default Destructor") << std::endl;
}

int User::getUserFd() {
    
    return this->_fd;
}

void	User::sendReply(std::string reply)
{
	reply.append("\r\n");
	size_t	total = 0;
	size_t	nbytes = reply.length();
	int		n;

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

void	User::welcome(void)
{
	sendReply(RPL_WELCOME(_nickname, _server, _username, _hostname));
	sendReply(RPL_YOURHOST(_nickname, _server));
	sendReply(RPL_CREATED(_nickname, _server, timestamp()));
	sendReply(RPL_MYINFO(_nickname, _server));
	_welcomed = true;
}

bool	User::hasBeenWelcomed() const{
	return (_welcomed);
}

std::string User::timestamp()
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];

	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
	
	return (buf);
}