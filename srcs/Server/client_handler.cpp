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
	std::cout << std::endl << "===== Doing client action " << clientSocket << std::endl;
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
		this->clients[]; // trouver le client qui a fait cette commande

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
		std::cout << "exemple " << tab.size() << std::endl;
		if (tab.size() > 0)
		{
			std::cout << "voici le message " << msg << std::endl;
			std::cout << "je rentre dans it != tab.begin()" << std::endl;
			// while (i < tmp.size())
			// {
			// 	tmp = tab[i].getSocketClient();
			// 	std::cout << "voici tous les sockets des clients " << tmp[i] << std::endl;
			// 	std::string msg_final = msg;
			// 	msg_final.replace("sa", "4");
			// 	send(tmp[i], msg.c_str(), msg.size() + 1, 0);
			// 	i++;
			// }
			// std::cout << "======";

			// 1[celui envois] 2[PRIVMSG] 3[celui qui recoit] 4[puis message]
			// <no>
			// light75018
			msg_client(clientSocket, tab, msg);
			std::cout << "======" << std::endl;
			// envoyer le message du serveur vers tous le monde
		}
		std::cout << "fin affichage" << std::endl;
	}
}

void Server::msg_client(int clientSocket, std::vector<Salon> tab, std::string msg)
{
	std::cout << "debut de la methode de msg_client" << std::endl;
	std::string envoyeur;
	std::string receveur;
	std::string final;
	unsigned long i = 0;
	//faire le brouillon du message ou il ne reste que les receveurs a ajouter
	// on decoupe etape par etape



	std::cout << "fin des definitions" << std::endl;
	(void)clientSocket;
	(void)msg;
	envoyeur = tab[i].getName();
	std::cout << "voici mon salon " << tab[i].getName() << std::endl; // le salon a le bon nom
	// Salon salon = tab[i];
	tab[i].show_list_client();
	std::cout << "nouveau test " << std::endl;
	std::cout << "par exemple " << tab[i].getName() << " mais encore ";
	Client client = tab[i].get_client(clientSocket);
	envoyeur = client.getNickname();
	std::cout << "voici envoyeur " << envoyeur;
	// std::cout << "ALOOOOOOOOOOOOOOOOOOOOOO " << tab[i].getName();
	size_t pos = msg.find('#');
	std::string message;
	if (pos < 2147483647)
		message = msg.substr(pos);
	else
		return ;
	std::cout << "premier substr dans msg_client" << std::endl;
	// pos = message.find(':');
	// message = message.erase(0, pos);
	// std::cout << "donc voici message " << message << std::endl; // donc #sq : sa par exemple
	// while (tab.size() > i)
	// {
	// 	std::cout << "voici les users et message diffuser: " << tab[i].getName() << " " << msg.c_str() << std::endl;
	// 	if (pos > 10000000)
	// 		return ;
	// 	std::cout << std::endl << std::endl;
	// 	std::cout << "voici mes taille pos " << pos << " msg " << msg.size() << std::endl; 
	// 	std::cout << "voici le message " << msg << std::endl;
	// 	std::string tmp;
	// 	try
	// 	{
	// 		std::cout << "je suis dans le try and catch" << std::endl;
	// 		std::stringstream nb_recev;
	// 		nb_recev << tmp[i];
	// 		receveur = nb_recev.str();
	// 		std::cout << "dasdsasd" << std::endl;
	// 		final = envoyeur + " PRIVMSG " + receveur + " " + message;
	// 		std::cout << "dsadasdasasdasdasddsadsasdsdassa" << std::endl;
	// 		std::cout << final << std::endl;
	// 		std::cout << "czxxxxxxxxxxxxxxxxxxdasdsasd" << std::endl;
	// 		int bytes = send(tmp[i], final.c_str(), msg.size() + 1, 0);
	// 		if (bytes == -1)
	// 			throw std::runtime_error("Error sending message");
	// 	}
	// 	catch(std::exception &e)
	// 	{
	// 		throw std::runtime_error("a problem happend when sending message");
	// 	}
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
	// }
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