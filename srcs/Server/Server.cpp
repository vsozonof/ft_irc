/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotilli <tpotilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:02:02 by vsozonof          #+#    #+#             */
/*   Updated: 2025/05/10 10:43:40 by tpotilli         ###   ########.fr       */
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

// int Server::setSalon(Salon salon, int i, int clientSocket)
// {
// 	size_t i = this->getSalon().size() - 1;
// 	std::cout << std::endl << "voici le nom du salon " << _salon[i].getName() << std::endl;
// 	_salon[i].increaseSocketClient(clientSocket);
// 	_salon[i].set_client(_clients, clientSocket);
// 	Client client = _salon[i].get_client(clientSocket);
// 	_salon[i].show_list_client();
// 	return 0;
// }

size_t Server::verif_Salon(Salon salon)
{
	int i = 0;
	size_t tmp = _salon.size();
	while (tmp > 0)
	{
		if (_salon[i].getName() == salon.getName())
		{
			std::cout << "salon is already existing" << std::endl;
			std::cout << "voici le nom du salon qui existe " << _salon[i].getName();
			std::cout << " et voici le i du channel " << i << std::endl;
			return i;
		}
		i++;
		tmp--;
	}
	std::cout << "juste apres verif salon " << std::endl;
	// for (int j = 0; (int)this->_salon.size() > j; j++)
	// {
	// 	std::cout << "salon numero " << j << " voici ses infos ====== :" << std::endl;
	// 	std::cout << this->_salon[j].getName() << " : "<< std::endl;
	// 	if (_salon[j].get_salon_client_len() > 0)
	// 		_salon[j].show_list_client();
	// }
	// std::cout << "voici les salon push " << salon.getName() << std::endl;
	// std::cout << "voici la len des salons " << _salon.size() << std::endl;
	_salon.push_back(salon);
	// std::cout << "voici la len des salons " << _salon.size() << std::endl;
	// for (int j = 0; (int)this->_salon.size() > j; j++)
	// {
	// 	std::cout << "salon numero " << j << " voici ses infos ====== :" << std::endl;
	// 	std::cout << this->_salon[j].getName() << " : "<< std::endl;
	// 	if (_salon[j].get_salon_client_len() > 0)
	// 		_salon[j].show_list_client();
	// }
	return 0;
}

std::vector<Salon> Server::getSalon(void)
{
	return _salon;
}

int Server::search_salon_by_socket(int clientSocket)
{
	size_t i = 0;
	std::cout << std::endl << " ====== DEBUT FONCTION SEARCH SALON ====== " << std::endl;
	while (_salon.size() > i)
	{
		size_t j = 0;
		(void)clientSocket;
		std::cout << std::endl << "et le i " << i << std::endl;
		std::map<int, Client> client = _salon[i].get_all_client();
		std::cout << "ok donc test " << client.size() << std::endl;
		std::cout << "Voici le socket qui va etre comparer " << clientSocket << " -> ";
		while (client[j].getSocket() != clientSocket && client.size() < j)
		{
			std::cout << client[j].getSocket() << " ";
			j++;
		}
		std::cout << "<-" << std::endl;
		if (client[j].getSocket() == clientSocket)
		{
			std::cout << std::endl << "donc voici socket " << clientSocket << client[j].getSocket();
			std::cout << std::endl << " FIN FONCTION SEARCH SALON I = " << i << std::endl;
			return i;
		}
		i++;
	}
	std::cout << std::endl << " ====== AUCUNE OCCURENCE DONC AUCUN SALON N'A ETE TROUVE ====== " << std::endl << std::endl;
	return -1;
}

// renvois la pos du salon ou il y a le user
// donc le probleme est que si aucune occurence est trouver c'est le salon 0 qui est
// selectionner donc mettre le -1 a la place
// je suis trop con il suffit de faire un find de socket pour comparer ou un truc du genre

void Server::run()
{
	while (42)
		handleClient();
}
