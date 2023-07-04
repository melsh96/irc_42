/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerClean.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meshahrv <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 17:40:05 by meshahrv          #+#    #+#             */
/*   Updated: 2023/07/04 17:40:30 by meshahrv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

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

/* Cleaning after ctrl^C */
void	Server::_clean() 
{
	for (std::map<int, User *>::iterator it = _user.begin(); it != _user.end(); ++it)
	{
		delete it->second;
	}
	for (pollfd_it it = _pollFd.begin(); it != _pollFd.end(); ++it)
	{
		close(it->fd);
	}
}
