/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: c2h6 <c2h6@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 10:58:08 by meshahrv          #+#    #+#             */
/*   Updated: 2023/07/04 12:24:47 by c2h6             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGES_HPP
# define MESSAGES_HPP

# include <string>

std::string RPL_WELCOME(std::string nick, std::string server, std::string user, std::string hostname);
std::string RPL_YOURHOST(std::string nick, std::string server);
std::string RPL_CREATED(std::string nick, std::string server , std::string date);
std::string RPL_MYINFO(std::string nick, std::string server);
std::string RPL_PONG(std::string source, std::string message);

std::string ERR_NOSUCHSERVER(std::string nick, std::string arg);
std::string ERR_PASSWDMISMATCH(std::string nick);
std::string ERR_NONICKNAMEGIVEN(std::string nick);
std::string ERR_NICKCOLLISION();
std::string ERR_ALREADYREGISTRED(std::string nick);
std::string ERR_NEEDMOREPARAMS(std::string nick, std::string arg);


#endif