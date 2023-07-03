/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meshahrv <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 14:10:46 by meshahrv          #+#    #+#             */
/*   Updated: 2023/07/03 14:28:48 by meshahrv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

Server::Server() {

	std::cout << GREEN("Default Constructor") << std::endl;

}

Server::Server(std::string port, std::string password) {
	
	this->_port = port;
	this->_password = password;
	
	std::cout << YELLOW("Server Constructor") << std::endl;
}

Server::~Server() {
	
	std::cout << RED("Default Destructor") << std::endl;
}