/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotilli <tpotilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:02:02 by vsozonof          #+#    #+#             */
/*   Updated: 2025/03/19 16:02:47 by tpotilli         ###   ########.fr       */
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

int Server::setSalon(Salon salon, int i)
{
	i = 0;
	size_t tmp = _salon.size();
	std::cout << "voici le nom du salon " << salon.getName() << "et size" << _salon.size() << std::endl;
	std::cout << "donc ici il y a un probleme" << std::endl;
	while (tmp > 0)
	{
		std::cout << "ok voici l'interieur tmp " << tmp << std::endl;
		if (_salon[i].getName() == salon.getName())
		{
			std::cout << "salon is already existing" << std::endl;
			return 1;
		}
		i++;
		tmp--;
	}
	std::cout << "le salon a bien ete set " << std::endl;
	_salon.push_back(salon);
	return 0;
}

int Server::verif_Salon(Salon salon)
{
	int i = 0;
	size_t tmp = _salon.size();
	while (tmp > 0)
	{
		if (_salon[i].getName() == salon.getName())
		{
			std::cout << "salon is already existing" << std::endl;
			return 1;
		}
		i++;
		tmp--;
	}
	_salon.push_back(salon);
	return 0;
}

std::vector<Salon> Server::getSalon(void)
{
	return _salon;
}

void Server::run()
{
	while (42)
		handleClient();
}
