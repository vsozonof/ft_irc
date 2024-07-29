/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 14:39:40 by vsozonof          #+#    #+#             */
/*   Updated: 2024/07/29 09:41:24 by vsozonof         ###   ########.fr       */
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
		exit (1);
	}

	Server server(atoi(argv[1]), argv[2]);
	server.run();
	
	return (0);
}