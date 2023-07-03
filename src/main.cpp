/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meshahrv <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 14:10:55 by meshahrv          #+#    #+#             */
/*   Updated: 2023/07/03 14:30:42 by meshahrv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../inc/Color.hpp"
#include "../inc/Server.hpp"

int main(int ac, char **av)
{
    (void)av;
    if (ac == 3)
    {
        std::cout << GREEN("DEBUG:") << "[INSIDE]" << std::endl;
        Server  server(av[1], av[2]);
    }
    else
    {
        std::cerr << RED("Error:") << " Try ./ircserv <port> <password>" << std::endl;
        return (1);
    }

    return (0);
}