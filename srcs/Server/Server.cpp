/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:02:02 by vsozonof          #+#    #+#             */
/*   Updated: 2024/07/23 17:20:45 by vsozonof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(unsigned int port, std::string password) : _port(port), _password(password)
{
	std::cout << BOLD_GREEN << "Server created" << def << std::endl;
	std::cout << "Port: " << _port << std::endl;
	std::cout << "Password: " << _password << std::endl;
}

Server::~Server()
{
	std::cout << BOLD_GREEN << "Server destroyed" << def << std::endl;
}

void Server::initServer()
{
	std::cout << BOLD_GREEN << "Server initialized" << def << std::endl;
	
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1)
		throw std::runtime_error(BOLD_RED "Socket creation failed" def);

	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(_port);
	servAddr.sin_addr.s_addr = INADDR_ANY;
	
	
}