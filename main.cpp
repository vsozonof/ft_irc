/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 14:39:40 by vsozonof          #+#    #+#             */
/*   Updated: 2024/11/06 04:38:48 by vsozonof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/inc.hpp"

int main (int argc, char **argv)
{
	if (parse(argv[1], argv[2], argc))
		return (1);
	try
	{
		Server server(atoi(argv[1]), argv[2]);
		server.run();
	}
	catch (std::exception &e)
	{
		std::cout << BOLD_RED << "⚠️  FATAL Error: " << e.what() << def << std::endl;
	}

	
	return (0);
}