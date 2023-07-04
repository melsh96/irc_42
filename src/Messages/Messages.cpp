/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: c2h6 <c2h6@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 10:57:59 by meshahrv          #+#    #+#             */
/*   Updated: 2023/07/04 12:24:36 by c2h6             ###   ########.fr       */
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

std::string RPL_PONG(std::string source, std::string message) {
	return ":" + source + " PONG :" + message;
}

std::string ERR_NOSUCHSERVER(std::string nick, std::string arg) {
	return "402 " + nick + ": " + arg + ":No such server";
}

std::string ERR_PASSWDMISMATCH(std::string nick) {
	return "464 " + (nick.empty() ?  "*" : nick) + " :Password incorrect";
}

std::string ERR_NONICKNAMEGIVEN(std::string nick) {
	return "431 " + (nick.empty() ?  "*" : nick) + ":No nickname given";
}

std::string ERR_NICKCOLLISION() {
	return "436 ";
}

std::string ERR_ALREADYREGISTRED(std::string nick) {
	return "462 " + nick + ":You may not reregister";
}

std::string ERR_NEEDMOREPARAMS(std::string nick, std::string arg) {
	return "461 " + (nick.empty() ?  "*" : nick) + " " + arg + " :Not enough parameters";
}