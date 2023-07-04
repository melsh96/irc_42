/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meshahrv <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 14:10:55 by meshahrv          #+#    #+#             */
/*   Updated: 2023/07/03 17:09:31 by meshahrv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../inc/Color.hpp"
#include "../inc/Server.hpp"

extern bool loop;

int main(int ac, char **av)
{
    loop = false;
    
    if (ac == 3)
    {
        loop = true;
        Server  server(av[1], av[2]);
    }
    else
    {
        std::cerr << RED("Error:") << " Try ./ircserv <port> <password>" << std::endl;
        return (1);
    }

    return (0);
}