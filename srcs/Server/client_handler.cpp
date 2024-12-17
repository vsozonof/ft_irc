/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 09:59:53 by vsozonof          #+#    #+#             */
/*   Updated: 2024/12/14 13:15:25 by vsozonof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <stdlib.h>

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

	std::cout << "premier substr " << std::endl;
	// ! Attention : quand find() fail, il retourne 18446744073709551615, ce qui fait peter le substr
	// ! Il faut donc verifier si find() a trouve qqch avant de faire le substr
	// ! Sinon, on peut faire find() + 1 pour eviter le probleme
	// ! J'ai aussi modif les parentheses dans mon if, pour que ca soit plus lisible
	// ! Voila tout, mon ami
	size_t pos = msg.find("PASS");
	size_t pos2 = msg.find("NICK");
	size_t pos3 = msg.find("USER");
	std::cout << "pos " << pos << " pos2 " << pos2 << " pos3 " << pos3 << std::endl;

	if (checkUserInfos(
		msg.substr((msg.find("PASS") + 5), _password.length()),
		msg.substr(msg.find("NICK"), msg.find("USER") - msg.find("NICK"))))
	{
		throw std::runtime_error("User infos not correct");
	}
	std::cout << "second substr " << std::endl;
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
		Salon salon(msg);
		setSalon(salon, 0);
		std::cout << "voici ma size " << _salon.size() << "dsadafaddssd" << std::endl;
		std::cout << "voici mon emplacement 0 " << _salon[i].getName() << std::endl;
		std::cout << " tst sddaasddsaasdadsasasddasasass" << std::endl;
		// std::map<int, Salon>::iterator it = _salon.begin();
		std::cout << "voici mon clientsocket " << clientSocket << std::endl;
		// std::vector<Salon>::iterator it = _salon.begin();
		std::cout << "tmp" << std::endl;
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
		std::vector<Salon> tab = getSalon();
		std::cout << "checkpoint " << std::endl;
		std::cout << "exemple " << tab.size() << std::endl;
		if (tab.size() > 0)
		{
			std::cout << "voici le message " << msg << std::endl;
			std::cout << "je rentre dans it != tab.begin()" << std::endl;
			int i = 0;
			std::vector<int> tmp = tab[i].getSocketClient();
			std::cout << "======";
			while (tmp[i])
			{
				std::cout << "voici les users et message diffuser: " << tmp[i] << " " << msg.c_str() << std::endl;
				size_t pos = msg.find('#');
				if (pos > 10000000)
					return ;
				std::cout << std::endl << std::endl;
				std::string final;
				std::string serv_name;
				std::string receveur;
				std::cout << "voici mes taille pos " << pos << " msg " << msg.size() << std::endl; 
				std::cout << "substr 1 " << std::endl; // un qui ne fonctionne pas ici
				try
				{
					std::string message = msg.substr(pos);
					pos = message.find(':');
					std::cout << "substr 2 " << std::endl;
					serv_name = message.substr(0, pos);
					std::cout << "substr 3 " << std::endl;
					message = message.erase(0, pos);
					std::cout << "donc voici message " << message << std::endl; // donc #sq : sa par exemple
					std::cout << "donc voici tmp2 " << serv_name << std::endl;
					std::istringstream(receveur) >> tmp[i];
					final = serv_name + "PRIVMSG " + message;
					std::cout << final << std::endl;
					int bytes = send(tmp[i], final.c_str(), msg.size() + 1, 0);
					if (bytes == -1)
						throw std::runtime_error("Error sending message");
				}
				catch(std::exception &e)
				{
					throw std::runtime_error("a problem happend when sending message");
				}
				// donc la j'ai mon message, le nom du serv, il me manque qu'a assembler tout
				// pour faire serv PRIVMSG receveur message
				// message set serv_name set
				// PRIVMSG doit etre mis a la mano
				// manque plus que le receveur
				std::cout << std::endl << std::endl;
				// int bytes = send(tmp[i], msg.c_str(), msg.size() + 1, 0);
				// if (bytes == -1)
				// 	throw std::runtime_error("Error sending message");
				std::cout << std::endl;
				i++;
			}
			std::cout << "======" << std::endl;
			// envoyer le message du serveur vers tous le monde
		}
		std::cout << "fin affichage" << std::endl;
	}
}

//essayer de faire serv PRIVMSG receveur : message

// Donc je dois cree mon serv 									-> fait
// Depuis un client, envoyer un message							-> fait
// Message recu par le serveur									-> fait
// Le serveur doit ensuite envoyer le message a tous le monde

// la j'ai PRIVMSG channel *le message*

// le probleme viens peut etre du formatage de ma commande
// -> utiliser les schema ci dessous

// schema de base	 envoyeur PRIVMSG receveur
// donc pour un serv Envoyeur PRIVMSG serv
// pour mp 			 Envoyeur PRIVmsg serv