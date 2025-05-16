/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 14:43:24 by vsozonof          #+#    #+#             */
/*   Updated: 2025/05/14 15:12:18 by vsozonof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INC_HPP
# define INC_HPP

#include <iostream>
#include <string>
#include <exception>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <map>
#include <poll.h>
#include <cstdio>
#include <netdb.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "../srcs/Server/Server.hpp"

#define BOLD_RED "\033[1;31m"
#define BOLD_GREEN "\033[1;32m"
#define BOLD_YELLOW "\033[1;33m"
#define def "\033[0m"

int	parse(std::string const &port, std::string const &password, int n_args);


#endif