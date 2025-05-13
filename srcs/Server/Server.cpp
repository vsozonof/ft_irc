/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotilli <tpotilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:02:02 by vsozonof          #+#    #+#             */
/*   Updated: 2025/05/11 19:01:09 by tpotilli         ###   ########.fr       */
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

size_t Server::verif_Salon(Salon salon)
{
	int i = 0;
	size_t tmp = _salon.size();
	while (tmp > 0)
	{
		if (_salon[i].getName() == salon.getName())
			return i;
		i++;
		tmp--;
	}
	salon.set_mode(0, 0);
	salon.set_mode(0, 2);
	salon.set_mode(0, 3);
	_salon.push_back(salon);
	return 0;
}

std::vector<Salon> Server::getSalon(void)
{
	return _salon;
}

int Server::search_salon_by_socket(int clientSocket)
{
	size_t i = 0;
	while (_salon.size() > i)
	{
		size_t j = 0;
		std::map<int, Client> client = _salon[i].get_all_client();
		while (client.size() > j)
		{
			if (client[j].getSocket() == clientSocket)
				return i;
			j++;
		}
		i++;
	}
	return -1;
}

void Server::run()
{
	while (42)
		handleClient();
}
