/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:02:02 by vsozonof          #+#    #+#             */
/*   Updated: 2024/07/29 07:28:21 by vsozonof         ###   ########.fr       */
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
	else if (listen(_socket, 5) == -1)
		throw std::runtime_error(BOLD_RED "Ft_IRC: Socket listen failed" def);

	struct pollfd serverPollFd;
	serverPollFd.fd = _socket;
	serverPollFd.events = POLLIN;
	_clients.push_back(serverPollFd);
}

void Server::run()
{
	std::cout << BOLD_GREEN << "Ft_IRC: Server running" << def << std::endl;
	
	while (1)
	{
		if (poll(_clients.data(), _clients.size(), -1) == -1)
			throw std::runtime_error(BOLD_RED "Ft_IRC: Poll failed" def);
	}
}

