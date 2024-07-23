/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 14:46:17 by vsozonof          #+#    #+#             */
/*   Updated: 2024/07/23 14:57:46 by vsozonof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/inc.hpp"

int	parse(std::string const &port, std::string const &password, int n_args)
{
	if (n_args != 3)
		throw std::invalid_argument(BOLD_RED "invalid number of arguments" def);
	else if (port.empty() || password.empty())
		throw std::invalid_argument(BOLD_RED "invalid arguments" def);
	else if (port.find_first_not_of("0123456789") != std::string::npos)
		throw std::invalid_argument(BOLD_RED "invalid port" def);

	std::cout << BOLD_GREEN << "Basic Parsing successful" << def << std::endl;
	std::cout << "Port: " << port << std::endl;
	std::cout << "Password: " << password << std::endl;
	return 0;
}
