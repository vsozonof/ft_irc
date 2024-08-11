/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 14:46:17 by vsozonof          #+#    #+#             */
/*   Updated: 2024/08/07 08:53:36 by vsozonof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/inc.hpp"

int parse(std::string const &port, std::string const &password, int n_args)
{
	if (n_args != 3)
	{
		std::cout << BOLD_RED "⚠️  Ft_IRC: Parsing: invalid number of arguments" def << std::endl;
		return 1;
	}
	else if (port.empty() || password.empty())
	{
		std::cout << BOLD_RED "⚠️  Ft_IRC: Parsing: invalid arguments" def << std::endl;
		return 1;
	}
	else if (port.find_first_not_of("0123456789") != std::string::npos)
	{
		std::cout << BOLD_RED "⚠️  Ft_IRC: Parsing: invalid port" def << std::endl;
		return 1;
	}
	else if (atoi(port.c_str()) < 1024 || atoi(port.c_str()) > 49151)
	{
		std::cout << BOLD_RED "⚠️  Ft_IRC: Parsing: invalid port range" def << std::endl;
		return 1;
	}
	std::cout << BOLD_GREEN "⚙️  Ft_IRC: Arguments parsed successfully" def << std::endl;
	return 0;
}
