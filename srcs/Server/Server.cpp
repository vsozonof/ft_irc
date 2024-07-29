/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:02:02 by vsozonof          #+#    #+#             */
/*   Updated: 2024/07/29 11:13:44 by vsozonof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(unsigned int port, std::string password) : _port(port), _password(password)
{
	std::cout << BOLD_GREEN << "Ft_IRC: Server created" << def << std::endl;
	try
	{
		initServer();
	}
	catch (std::exception &e)
	{
		std::cerr << BOLD_RED << "Ft_IRC: Error: " << e.what() << def << std::endl;
	}
}

Server::~Server()
{
	std::cout << BOLD_GREEN << "Ft_IRC: Server destroyed" << def << std::endl;
}

void Server::initServer()
{
	std::cout << BOLD_GREEN << "Ft_IRC: Server initialized" << def << std::endl;
	
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1)
		throw std::runtime_error(BOLD_RED "Ft_IRC: Socket creation failed" def);

	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(_port);
	servAddr.sin_addr.s_addr = INADDR_ANY;
	
	if (bind(_socket, (sockaddr *)&servAddr, sizeof(servAddr)) == -1)
		throw std::runtime_error(BOLD_RED "Ft_IRC: Socket bind failed" def);
	
	if (listen(_socket, 10) == -1)
		throw std::runtime_error(BOLD_RED "Ft_IRC: Socket listen failed" def);

}

void Server::run()
{
	while (42)
		acceptClient();
}
