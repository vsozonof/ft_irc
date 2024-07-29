/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 14:39:40 by vsozonof          #+#    #+#             */
/*   Updated: 2024/07/29 07:22:27 by vsozonof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/inc.hpp"

int main (int argc, char **argv)
{
	try
	{
		parse(argv[1], argv[2], argc);
	}
	catch (std::exception &e)
	{
		std::cout << BOLD_RED << "Error: " << e.what() << def << std::endl;
		return (1);
	}

	Server server(atoi(argv[1]), argv[2]);
	server.run();
	
	return (0);
}