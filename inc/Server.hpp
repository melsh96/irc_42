/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meshahrv <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 14:10:07 by meshahrv          #+#    #+#             */
/*   Updated: 2023/07/03 14:23:50 by meshahrv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>

#include "Color.hpp"

class Server {
	
	private:
		std::string		_port;
		std::string		_password;
		
	public:
		Server();
		Server(std::string port, std::string password);
		~Server();
	
};

#endif