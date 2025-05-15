/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:02:02 by vsozonof          #+#    #+#             */
/*   Updated: 2025/05/15 17:30:18 by vsozonof         ###   ########.fr       */
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
	std::cout << BOLD_GREEN << "🗑️  Ft_IRC: Server destroyed" << def << std::endl;
}

void Server::initServer()
{
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1)
		throw std::runtime_error(BOLD_RED "Socket creation failure" def);

	int opt = 1;
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw std::runtime_error("setsockopt(SO_REUSEADDR) failure");

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
	_shutdown = 0;
	std::cout << BOLD_GREEN << "⚙️  Ft_IRC: Server successfully initialized" << def << std::endl;
}

void Server::initServerShutdown() {
	_shutdown = 1;
}

Server* g_server = NULL;

void sigHandler(int s)
{
	(void)s;
	g_server->initServerShutdown();
}

void Server::initSigHandler() {
	struct sigaction sigIntHandler;
	
	sigIntHandler.sa_handler = sigHandler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	
	sigaction(SIGINT, &sigIntHandler, NULL);
	
	g_server = this;
}

void Server::run()
{	
	initSigHandler();
	while (!_shutdown)
		handleClient();
	std::cout << BOLD_GREEN << "\n⚙️  Ft_IRC: Successfully stopped the server!" << std::endl;
	close(_socket);
}
