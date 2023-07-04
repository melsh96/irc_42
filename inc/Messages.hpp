/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meshahrv <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 10:58:08 by meshahrv          #+#    #+#             */
/*   Updated: 2023/07/04 10:59:25 by meshahrv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGES_HPP
# define MESSAGES_HPP

# include <string>

std::string RPL_WELCOME(std::string nick, std::string server, std::string user, std::string hostname);
std::string RPL_YOURHOST(std::string nick, std::string server);
std::string RPL_CREATED(std::string nick, std::string server , std::string date);
std::string RPL_MYINFO(std::string nick, std::string server);

#endif