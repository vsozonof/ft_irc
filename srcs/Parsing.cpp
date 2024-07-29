/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 14:46:17 by vsozonof          #+#    #+#             */
/*   Updated: 2024/07/29 08:11:54 by vsozonof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/inc.hpp"

int	parse(std::string const &port, std::string const &password, int n_args)
{
	if (n_args != 3)
		throw std::invalid_argument(BOLD_RED "Ft_IRC: invalid number of arguments" def);
	else if (port.empty() || password.empty())
		throw std::invalid_argument(BOLD_RED "Ft_IRC: invalid arguments" def);
	else if (port.find_first_not_of("0123456789") != std::string::npos)
		throw std::invalid_argument(BOLD_RED "Ft_IRC: invalid port" def);
	else if (atoi(port.c_str()) < 1024 || atoi(port.c_str()) > 49151)
		throw std::invalid_argument(BOLD_RED "Ft_IRC: invalid port range" def);
	std::cout << BOLD_GREEN << "Ft_IRC: Basic Parsing successful" << def << std::endl;
	return 0;
}
