/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 09:59:53 by vsozonof          #+#    #+#             */
/*   Updated: 2024/11/12 17:17:10 by vsozonof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::handleClient()
{
	int ret = poll(_fds.data(), _fds.size(), -1);

	if (ret > 0)
	{
		for (long unsigned int i = 0; i < _fds.size(); i++)
		{
			if (_fds[i].revents & POLLIN)
			{
				if (_fds[i].fd == _socket)
				{
					int clientSocket = accept(_socket, NULL, NULL);
					if (clientSocket == -1)
						throw std::runtime_error("Socket accept() failure");
					setupNewClient(clientSocket);
				}
				else
				{
					int clientSocket = _fds[i].fd;
					doClientAction(clientSocket);
				}
			}
		}
	}
	else if (ret == -1)
		throw std::runtime_error("Poll error");
}

void Server::setupNewClient(int clientSocket)
{
	std::cout << "\033[1m" << "Setting up new client" << std::endl;
	Client newClient(clientSocket);
	_clients.insert(std::pair<int, Client>(clientSocket, newClient));

	pollfd newClient_fd;
	newClient_fd.fd = clientSocket;
	newClient_fd.events = POLLIN;
	_fds.push_back(newClient_fd);

	std::string msg;
	
	int check = 0;
	while (!check)
	{
		msg += _clients[clientSocket].receiveMsg();
		std::cout << clientSocket << " Received: " << msg << std::endl;
		if (msg.find("USER") != std::string::npos
			&& msg.find("NICK") != std::string::npos
			&& msg.find("PASS") != std::string::npos)
		{
			std::cout << "All infos received" << std::endl;
			check = 1;
		}
		else
		{
			std::cout << "Not all infos received - implement TO" << std::endl;
		}
	}

	if (checkUserInfos(msg.substr((msg.find("PASS") + 5), _password.length()),
		msg.substr((msg.find("NICK"), msg.find("USER") - msg.find("NICK")))))
	{
		throw std::runtime_error("User infos not correct");
	}
	_clients[clientSocket].sendMsg(":127.0.0.1 001 test :Welcome to the IRC Network\r\n");
}

// ! Do-Client Action = fonction qui va gerer les cmd client, les messages...
void Server::doClientAction(int clientSocket)
{
	std::cout << "Doing client action " << clientSocket << std::endl;
	char buffer[1024];
    int bytesRecv = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesRecv <= 0)
    {
		std::cout << "Client " << clientSocket << " disconnected" << std::endl;
        _clients[clientSocket].closeClient();
        _clients.erase(clientSocket);
        return;
    }

    buffer[bytesRecv - 1] = '\0';
    std::string msg(buffer);

	std::cout << clientSocket << " Received: " << msg << std::endl;
	if (msg.find("JOIN") != std::string::npos)
	{
		std::cout << "JOIN command" << std::endl;
		// ajouter la creation du salon a ce moment la
		msg = msg.erase(0, 6);
		size_t i = 0;
		std::cout << "voici mon nouveau msg " << msg << std::endl;
		Salon salon (msg);
		_salon.insert(std::pair<int, Salon>(i, salon));
		std::cout << "voici ma size " << _salon.size() << std::endl;
		std::cout << "voici mon emplacement 0 " << _salon[i].getName() << std::endl;
		std::cout << " tst sddaasddsaasdadsasasddasasass" << std::endl;
		// std::map<int, Salon>::iterator it = _salon.begin();
		_salon[i].increaseSocketClient(clientSocket);
		// donc la en theorie: mon salon avec son nom est cree, je dois maintenant stocker
		// les gens qui entrent dedans pour pouvoir leurs faire parvenir le message
	}
	else if (msg.find("PING") != std::string::npos)
	{
		std::string pong = "PONG localhost\r\n";
		_clients[clientSocket].sendMsg(pong);
	}
	else if (msg.find("QUIT") != std::string::npos)
	{
		std::cout << "QUIT command" << std::endl;
	}
	else
	{
		// faire dire au chat le message, ca doit etre visible pour les autres
		std::cout << "Unknown command" << std::endl;
		_clients[clientSocket].sendMsg("Unknown command\r\n");
		std::map<int, Salon> tab = getSalon();
		std::map<int, Salon>::iterator it = tab.begin();
		std::cout << "checkpoint " << std::endl;
		if (it != tab.begin())
		{
			std::cout << "je rentre dans it != tab.begin()" << std::endl;
			while (it != tab.begin())
			{
				int bytes = send(it != tab.begin(), msg.c_str(), msg.size() + 1, 0);
				if (bytes == -1)
					throw std::runtime_error("Error sending message");
			}
		}
		std::cout << "fin affichage" << std::endl;
	}
}
