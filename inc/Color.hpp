/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Color.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: c2h6 <c2h6@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 14:14:37 by meshahrv          #+#    #+#             */
/*   Updated: 2023/07/04 11:58:11 by c2h6             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLOR_HPP
#define COLOR_HPP

#define NC "\033[0m"

#define GREEN(s) "\033[1;38;5;150m" s "\033[m"
#define BLUE(s) "\033[1;36m" s "\033[m"
#define RED(s) "\033[1;31m" s "\033[m"

#define YELLOW(s) "\033[1;33m" s "\033[m"
#define VIOLET(s) "\033[38;5;140m" s "\033[m"
#define WHITE(s) "\033[1;37m" s "\033[m"
#define GREY(s) "\033[1;90m" s "\033[m"

#define rouge "\033[1;31m"
#define vert "\033[1;32m"
#define fin "\033[0m"

#endif