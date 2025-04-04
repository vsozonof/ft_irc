/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotilli <tpotilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 09:59:53 by vsozonof          #+#    #+#             */
/*   Updated: 2025/03/26 21:09:16 by tpotilli         ###   ########.fr       */
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
	std::cout << "\033[1m" << "----- Setting up new client ------" << std::endl;
	std::cout << "Setting up client: " << clientSocket << std::endl;
	
	Client newClient(clientSocket);
	_clients.insert(std::pair<int, Client>(clientSocket, newClient));
	pollfd newClient_fd;
	newClient_fd.fd = clientSocket;
	newClient_fd.events = POLLIN;
	_fds.push_back(newClient_fd);

	std::string msg;
	std::cout << '[' << clientSocket << ']' << ": Gathering data" << std::endl;
	int check = 0;
	while (!check)
	{
		msg += _clients[clientSocket].receiveMsg();
		if (msg.find("USER") != std::string::npos
			&& msg.find("NICK") != std::string::npos
			&& msg.find("PASS") != std::string::npos)
		{
			std::cout << '[' << clientSocket << ']' << ": All informations successfully collected" << std::endl;
			check = 1;
		}
		else
		{
			std::cout << '[' << clientSocket << ']' << ": Missing informations.." << std::endl;
		}
	}

	// ! Attention : quand find() fail, il retourne 18446744073709551615, ce qui fait peter le substr
	// ! Il faut donc verifier si find() a trouve qqch avant de faire le substr
	// ! Sinon, on peut faire find() + 1 pour eviter le probleme
	// ! J'ai aussi modif les parentheses dans mon if, pour que ca soit plus lisible
	// ! Voila tout, mon ami
	std::string userPass = msg.substr((msg.find("PASS") + 5), _password.length());
	std::string userNick = extractValue(msg, "NICK");
	std::string userName = extractValue(msg, "USER");

	std::cout << "______________________________________" << std::endl;
	std::cout << "\n[" << clientSocket << "]: " << "INFOS RECEIVED:\n" 
				<< "PASS: " << '[' << userPass << ']'
				<< "\nNICK: " << '[' << userNick << ']' 
				<< "\nNAME: " << '[' << userName << ']';
	std::cout << "\n------------------------------------" << '\n';

	
	if (checkUserInfos(userPass, userNick))
		throw std::runtime_error("User infos not correct"); // ! Fix le retour d'erreur

	_clients[clientSocket].setNickname(userNick);
	_clients[clientSocket].setUsername(userName);
	
	
	// std::cout << "______________________________________" << std::endl;
	// std::cout << "NICKNAME =" << _clients[clientSocket].getNickname() << std::endl;
	// std::cout << "USERNAME =" << _clients[clientSocket].getUsername() << std::endl;
	// std::cout << "CLIENTSOCKET =" << _clients[clientSocket].getSocket() << std::endl;
	_clients[clientSocket].sendMsg(":127.0.0.1 001 test :Welcome to the IRC Network AFWEIOPXJWEPFOIJA\r\n");
}

// ! Do-Client Action = fonction qui va gerer les cmd client, les messages...

void Server::doClientAction(int clientSocket)
{
	std::cout << std::endl << "===== Doing action of client " << clientSocket << "======" << std::endl;
	std::cout << "donc voici les infos clients suivant sont socket " << clientSocket << std::endl;
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
		std::cout << "ETAPE 1 CREE MON SALON " << std::endl << std::endl;
		// ajouter la creation du salon a ce moment la
		msg = msg.erase(0, 6);
		size_t i = 0;
		Salon salon(msg);
		if (setSalon(salon, 0) == 1)
			std::cout << "he exit" << std::endl;
		_salon[i].increaseSocketClient(clientSocket);
		std::cout << "voici le client que j'envois " << _clients[clientSocket].getNickname() << std::endl;
		_salon[i].set_client(_clients, clientSocket);
		int socket_tmp = clientSocket;
		for(int j = _salon[i].get_salon_client_len(); j > 0; j--)
		{
			std::cout << "liste client ⚠️⚠️" << this->_clients[socket_tmp].getNickname() << std::endl;
			socket_tmp++;
		}
		_salon[i].show_list_client();
		_salon[i].showMessage();
		Client client = _salon[i].get_client(clientSocket, _clients);
		_salon[i].show_client_infos(clientSocket);
		std::cout << "et maintenant verif si dans le serv les infos sont bien enregistrer" << std::endl;
		std::cout << _salon[i].getName() << std::endl;
		std::cout << "nick " << _clients[clientSocket].getNickname();
		std::cout << " | user " << _clients[clientSocket].getUsername();
		std::cout << " cc " << clientSocket << std::endl;
		send(clientSocket, msg.c_str(), msg.size(), 0);
		int bytes = send(clientSocket, msg.c_str(), msg.size(), 0);
			if (bytes == -1)
				throw std::runtime_error("Error sending message with send");
		_salon[i].show_client_infos(clientSocket);
		std::cout << "FIN DE LA CREATION DU NOUVEAU SALON DONC FIN ETAPE 1" << std::endl << std::endl;
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
		std::cout << "je rentre dans le ELSEEEEEEEEEEEEEE DONC ETAPE 2:" << std::endl << std::endl;
		// std::cout << msg << std::endl;
		int i = 0;
		for (; (int)this->_salon.size() > i; i++)
		{
			std::cout << "salon numero " << i << " voici ses infos ====== :" << std::endl;
			std::cout << this->_salon[i].getName() << " : "<< std::endl;
			_salon[i].show_list_client();
			std::cout << "======" << std::endl;
		}
		if (_salon.size() > 0)
		{
			std::cout << i << std::endl;
			// Salon tab = _salon[0];
			// std::cout << "nom " << tab.getName() << std::endl;
			// std::cout << "je suis rentrer dans le if des salons" << std::endl;
			// std::map <int, Client> client = tab.get_all_client();
			// std::cout << "=============" << std::endl;
			// std::cout << "nick " << client[clientSocket].getNickname();
			// std::cout << " | user " << client[clientSocket].getUsername();
			// std::cout << " cc " << clientSocket << std::endl;
			// std::cout << "=============" << std::endl;
			// Salon tab2 = _salon[0];
			// std::cout << "voici le salon qui est recup [" << tab2.getName() << "]" << std::endl;
			// Client client = tab2.get_client(clientSocket, _clients);
			// std::cout << "verification du client:" << std::endl;
			// std::cout << "nick [" << client.getNickname() << "]";
			// std::cout << " user [" << client.getUsername() << "]" << std::endl;
			// std::cout << "verification du client via le salon:" << std::endl;
			// _salon[0].show_list_client();
			// std::cout << "balise n1" << std::endl;
			// tab2.show_list_client();
			// std::cout << "balise n2" << std::endl;
			// tab.show_list_client();
			std::cout << "et voici le nom de la copie" << std::endl;
			// std::cout << tab.getName();
			// std::cout << tab.get_SocketClient(0) << std::endl;
			// std::cout << "voici le message " << msg << std::endl;
			// std::cout << "je rentre dans it != tab.begin()" << std::endl;
			_salon[0].show_list_client();
			msg_client(clientSocket, _salon[0], msg);
			std::cout << "FIN DU ELSE DONC FIN ETAPE 2" << std::endl << std::endl;
		}

		// 	// 1[celui envois] 2[PRIVMSG] 3[celui qui recoit] 4[puis message]
		// 	// <no>
		// 	// light75018
		std::cout << "fin affichage" << std::endl;
	}
}

void Server::msg_client(int clientSocket, Salon &tab, std::string msg)
{
	// std::cout << "debut de la methode de msg_client" << std::endl;
	std::string envoyeur;
	std::string final;
	int i = 0;
	(void)tab;
	//faire le brouillon du message ou il ne reste que les receveurs a ajouter
	// on decoupe etape par etape

	std::cout << std::endl << "==== INFO POUR COMMENT LE MSG VA ETRE ENVOYE ====" << std::endl << std::endl;
	std::cout << "ENVOIS DE LA PART DE " << clientSocket << " ET VOICI LE MESSAGE " << std::endl;
	std::cout << msg << std::endl;
	msg = msg.erase(msg.size() - 1);
	tab.show_list_client();
	(void)clientSocket;
	// std::cout << "voici mon salon " << envoyeur << std::endl; // le salon a le bon nom
	// std::cout << " mais encore 🤩🤩🤩🤩🤩" << std::endl;
	// std::cout << "=== verif salon et client" << std::endl;
	// Client client = tab[i].get_client(clientSocket, _clients);
	// std::cout << "voici le channel " << tab[i].getName();
	// tab[i].show_list_client();
	// std::cout << "donc voici les infos clients suivant sont socket " << _clients[clientSocket].getSocket();
	// std::cout << " nick " << _clients[clientSocket].getNickname();
	// std::cout << " et " << _clients[clientSocket].getUsername() << std::endl;
	// std::cout << "=== verif salon et client === fin" << std::endl;
	envoyeur = tab.getName();
	// std::cout << std::endl << "voici envoyeur " << envoyeur << std::endl;
	// std::cout << "ALOOOOOOOOOOOOOOOOOOOOOO " << tab[i].getName();
	std::cout << "voici le message de base " << msg << std::endl;
	std::cout << "donc voici le message de base " << msg << "|" << std::endl; // donc #sq : sa par exemple
	int pos = msg.find(":");
	if (pos > 2147483647 || pos < 0)
		return;
	std::cout << "je suis apres et voici pos " << pos << std::endl;
	// std::string message;
	// std::cout << "voici pos " << pos << std::endl;
	// std::cout << "voici len " << msg.size() - pos << std::endl;
	// message = msg.substr(pos, msg.size() - pos);
	// std::cout << "voici message " << message;
	std::cout << tab.getName() << " '"<< std::endl;
	// final = ":server_name PRIVMSG #";
	// final.append(tab.getName());
	// final.append(":");
	// final.append(message);
	// final.append("\r\n");
	std::string nv = ":";
	std::cout << "voici nv " << nv << std::endl;
	std::map<int, Client> client = tab.get_all_client();
	
	tab.show_list_client();
	std::cout << clientSocket;
	std::cout << " et voici le client " << client[clientSocket].getNickname() << std::endl;
	// std::cout << .getNickname() << std::endl;
	nv.append(client[clientSocket].getUsername());
	std::cout << "voici nv " << nv << std::endl;
	nv.append(" " + msg);
	std::cout << "voici nv " << nv << std::endl;
	nv.append("\r\n");
	std::cout << "nv = ";
	std::cout << nv << std::endl;
	std::cout << "voila le message a ete envoye sa mere" << std::endl;
	// std::string msg1 = ":server_name PRIVMSG #sa :Hello, IRSSI!\r\n";
	// std::cout << final << std::endl;
	while (tab.get_salon_client_len() > i)
	{
		std::cout << std::endl << std::endl;
		try
		{
			std::cout << "liste client dans le salon " << std::endl;
			tab.show_list_client();
			while (tab.get_SocketClient(i) > 0)
			{
				std::cout << tab.get_SocketClient(i) << std::endl;
				std::cout << "isolation" << std::endl;
				tab.show_client_infos(tab.get_SocketClient(i));
				std::cout << "isolation" << std::endl;
				std::cout << "voici final " << std::endl;
				std::cout << final << std::endl;
				int bytes = send(tab.get_SocketClient(i), nv.c_str(), nv.size(), 0);
				if (bytes == -1)
					throw std::runtime_error("Error sending message with send");
				i++;
				if (i >= tab.get_salon_client_len())
					break;
			}
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
		std::cout << "==== FIN DES INFOS POUR COMMENT LE MSG VA ETRE ENVOYER ====" << std::endl << std::endl;
		// int bytes = send(tmp[i], msg.c_str(), msg.size() + 1, 0);
		// if (bytes == -1)
		// 	throw std::runtime_error("Error sending message");
		std::cout << std::endl;
		i++;
	}
}

// std::string msg = ":server_name PRIVMSG #sa :Hello, IRSSI!\r\n";
// donc 			 ":server_name PRIVMSG #channel : *le message*"
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

//Thus, there are 510 characters maximum allowed for the command and its parameters