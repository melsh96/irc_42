/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meshahrv <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 10:57:59 by meshahrv          #+#    #+#             */
/*   Updated: 2023/07/04 11:00:04 by meshahrv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Messages.hpp"

std::string RPL_WELCOME(std::string nick, std::string server, std::string user, std::string hostname){
	return ":" + server + " 001 " + nick + " :Welcome to the " + server + " network, " + nick + "[" + user + "@" + hostname + "]";
}

std::string RPL_YOURHOST(std::string nick, std::string server){
	return ":" + server + " 002 " + nick + " :Your host is " + server + ", running version 1.2.3";
}

std::string RPL_CREATED(std::string nick, std::string server , std::string date){
	return ":" + server + " 003 " + nick + " :This server was created " + date;
}

std::string RPL_MYINFO(std::string nick, std::string server){
	return ":" + server + " 004 " + nick + " " + server + " irssi 1.2.3 (20210409 0011)" ;
}
