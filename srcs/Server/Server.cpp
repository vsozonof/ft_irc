/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:02:02 by vsozonof          #+#    #+#             */
/*   Updated: 2024/11/08 09:50:08 by vsozonof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(unsigned int port, std::string password) : _port(port), _password(password)
{
	std::cout << BOLD_GREEN << "⚙️  Ft_IRC: Creating IRC Server ..." << def << std::endl;
	initServer();
}

Server::~Server()
{
	std::cout << BOLD_GREEN << "Ft_IRC: Server destroyed" << def << std::endl;
}

void Server::initServer()
{
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1)
		throw std::runtime_error(BOLD_RED "Socket creation failure" def);
	
	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(_port);
	servAddr.sin_addr.s_addr = INADDR_ANY;
	
	if (bind(_socket, (sockaddr *)&servAddr, sizeof(servAddr)) == -1)
		throw std::runtime_error(BOLD_RED "Socket bind() failure" def);
	
	if (listen(_socket, 10) == -1)
		throw std::runtime_error(BOLD_RED "Socket listen() failure" def);

	pollfd listening_fd;
	listening_fd.fd = _socket;
	listening_fd.events = POLLIN;
	_fds.push_back(listening_fd);
	std::cout << BOLD_GREEN << "⚙️  Ft_IRC: Server successfully initialized" << def << std::endl;
}

void Server::setSalon(Salon salon, int i)
{
	(void)i;
	_salon.push_back(salon);
}

std::vector<Salon> Server::getSalon(void)
{
	std::cout << "SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS" << std::endl;
	std::cout << this->_salon[0].getName() << std::endl;
	return _salon;
}

void Server::run()
{
	while (42)
		handleClient();
}
