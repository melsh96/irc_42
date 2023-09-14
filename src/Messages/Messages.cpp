/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbily <fbily@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 10:57:59 by meshahrv          #+#    #+#             */
/*   Updated: 2023/09/14 20:43:54 by fbily            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Messages.hpp"

//"<client> :Welcome to the <networkname> Network, <nick>[!<user>@<host>]"
std::string RPL_WELCOME(std::string nick, std::string server, std::string user, std::string hostname){
	return ":" + server + " 001 " + nick + " :Welcome to the " + server + " network, " + nick + "[" + user + "@" + hostname + "]";
}

//"<client> :Your host is <servername>, running version <version>"
std::string RPL_YOURHOST(std::string nick, std::string server){
	return ":" + server + " 002 " + nick + " :Your host is " + server + ", running version 1.2.3";
}

//"<client> :This server was created <datetime>"
std::string RPL_CREATED(std::string nick, std::string server , std::string date){
	return ":" + server + " 003 " + nick + " :This server was created " + date;
}

//"<client> <servername> <version> <available user modes> <available channel modes> [<channel modes with a parameter>]"
std::string RPL_MYINFO(std::string nick, std::string server){
	return ":" + server + " 004 " + nick + " " + server + " irssi 1.2.3 (20210409 0011)" ;
}

// ??
std::string RPL_PONG(std::string source, std::string message) {
	return ":" + source + " PONG :" + message;
}

//"<client> <channel> :<topic>"
std::string RPL_TOPIC(std::string nick, std::string server, std::string channel, std::string topic){
	return (":" + server + " 332 " + nick + ' ' + channel + " :" + topic);
}

//"<client> <channel> :No topic is set"
std::string RPL_NOTOPIC(std::string nick, std::string server, std::string channel){
	return (":" + server + " 331 " + nick + ' ' + channel + " :No topic is set");
}

//"<client> <channel> <nick> <setat>"
std::string RPL_TOPICWHOTIME(std::string nick, std::string server, std::string channel, std::string user, std::string date){
	return (":" + server + " 333 " + nick + ' ' + channel + ' ' + user + ' ' + date);
}

//"<client> <symbol> <channel> :[prefix]<nick>{ [prefix]<nick>}"
std::string RPL_NAMREPLY(std::string nick, std::string server, std::string channel, std::string user){
	return (":" + server + " 353 " + nick + " * " + channel + ':' + user);
}

//"<client> <channel> :End of /NAMES list"
std::string RPL_ENDOFNAMES(std::string nick, std::string server, std::string channel){
	return (":" + server + " 366 " + nick + ' ' + channel + " :End of NAMES list");
}

//"<client> <nick> <channel>"
std::string RPL_INVITING(std::string nick, std::string server, std::string channel, std::string user){
	return (":" + server + " 341 " + nick + ' ' + user + ' ' + channel);
}

//"<client> <channel> <modestring> <mode arguments>..."
std::string RPL_CHANNELMODEIS(std::string server, std::string client, std::string channel, std::string modestring){
	return (":" + server + " 324 " + client + ' ' + channel + ' ' + modestring);
}

//"<client> <channel> <modestring> <mode arguments>..."
std::string RPL_CREATIONTIME(std::string server, std::string client, std::string channel, std::string creationtime){
	return (":" + server + " 329 " + client + ' ' + channel + ' ' +  creationtime);
}

//"<client> <server name> :No such server"
std::string ERR_NOSUCHSERVER(std::string nick, std::string arg) {
	return "402 " + nick + ": " + arg + " :No such server";
}

//"<client> :Password incorrect"
std::string ERR_PASSWDMISMATCH(std::string nick) {
	return "464 " + (nick.empty() ?  "*" : nick) + " :Password incorrect";
}

//"<client> :No nickname given"
std::string ERR_NONICKNAMEGIVEN(std::string nick) {
	return "431 " + (nick.empty() ?  "*" : nick) + " :No nickname given";
}

//"<client> <nick> :Nickname collision KILL from <user>@<host>"
std::string ERR_NICKCOLLISION(std::string nick) {
	return "436 " + nick + " :Nickname is already used";
}

//"<client> :You may not reregister"
std::string ERR_ALREADYREGISTERED(std::string nick) {
	return "462 " + nick + " :You may not reregister";
}

//"<client> <command> :Not enough parameters"
std::string ERR_NEEDMOREPARAMS(std::string nick, std::string arg) {
	return "461 " + (nick.empty() ?  "*" : nick) + " " + arg + " :Not enough parameters";
}

//"<client> <channel> :Cannot join channel (+k)"
std::string ERR_BADCHANNELKEY(std::string nick, std::string channel){
	return ("475 " + nick + ' ' + channel + " :Cannot join channel (+k)");
}

//"<client> <channel> :Cannot join channel (+l)"
std::string ERR_CHANNELISFULL(std::string nick, std::string channel){
	return ("471 " + nick + ' ' + channel + " :Cannot join channel (+l)");
}

//"<client> <channel> :Cannot join channel (+i)"
std::string ERR_INVITEONLYCHAN(std::string nick, std::string channel){
	return ("473 " + nick + ' ' + channel + " :Cannot join channel (+i)");
}

//"<client> <channel> :No such channel"
std::string ERR_NOSUCHCHANNEL(std::string nick, std::string channel){
	return ("403 " + nick + ' ' + channel + " :No such channel");
}

//"<client> <channel> :You're not channel operator"
std::string ERR_CHANOPRIVSNEEDED(std::string nick, std::string channel){
	return ("482 " + nick + ' ' + channel + " :You're not channel operator");
}

//"<client> <nick> <channel> :They aren't on that channel"
std::string ERR_USERNOTINCHANNEL(std::string nick, std::string channel, std::string user){
	return ("441 " + nick + ' ' + user + ' ' + channel + " :They aren't on that channel");
}

//"<client> <channel> :You're not on that channel"
std::string ERR_NOTONCHANNEL(std::string nick, std::string channel){
	return ("442 " + nick + ' ' + channel + " :You're not on that channel");
}

//"<client> <nick> <channel> :is already on channel"
std::string ERR_USERONCHANNEL(std::string nick, std::string channel, std::string user){
	return ("443 " + nick + ' ' + user + ' ' + channel + " :is already on channel");
}

//"<client> <modechar> :is unknown mode char to me"
std::string ERR_UNKNOWNMODE(std::string nick, std::string modechar){
	return ("472 " + nick + ' ' + modechar);
}
//"<client> <nickname> :No such nick/channel"
std::string ERR_NOSUCHNICK(std::string nick, std::string user)
{
	return ("401 " + nick + " " + user + " :No such nick/channel");
}

//"<client> <command>{ <subcommand>} :<info>"
std::string	ERR_UNKNOWNERROR(std::string nick, std::string server, std::string hostname, std::string cmd, std::string comment)
{
	return (":" + server + " 400 " + nick + "!" + nick + "@" + hostname + ' ' + cmd + " :" + comment);
}
//"<client> <channel> :Cannot send to channel"
std::string	ERR_CANNOTSENDTOCHAN(std::string nick, std::string channel)
{
	return ("404 " + nick + " " + channel + " :Cannot send to channel");
}

//"<client> :No text to send"
std::string ERR_NOTEXTTOSEND(std::string nick)
{
	return ("412 " + nick + " :No text to send");
}