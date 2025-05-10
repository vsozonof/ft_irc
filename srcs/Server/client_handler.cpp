/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotilli <tpotilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 09:59:53 by vsozonof          #+#    #+#             */
/*   Updated: 2025/05/10 11:04:08 by tpotilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <stdlib.h>
#include "../commands/command.hpp"

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

void Server::deleteClient(int clientSocket)
{
	close(clientSocket);
    for (std::vector<pollfd>::iterator it = _fds.begin(); it != _fds.end(); ++it)
    {
        if (it->fd == clientSocket)
        {
            _fds.erase(it);
            break;
        }
    }
    _clients.erase(clientSocket);
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
	fcntl(clientSocket, F_SETFL, O_NONBLOCK);

	std::string msg;
	std::cout << '[' << clientSocket << ']' << ": Gathering data" << std::endl;
	int check = 0;
	int timeout = 0;
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
			sleep(1);
			std::cout << '[' << clientSocket << ']' << ": Missing informations.." << std::endl;
			timeout++;
			if (timeout == 5) 
			{
				std::cout << '[' << clientSocket << ']' << ": TimeOut, disconnecting client.." << std::endl;
				deleteClient(clientSocket);
				return ;
			}
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

	if (checkPassword(userPass))
	{
		_clients[clientSocket].sendMsg(":127.0.0.1 464 * :Password incorrect\r\n");
		deleteClient(clientSocket);
		return ;
	}
	else if (checkNick(userName))
	{
		_clients[clientSocket].sendMsg(":127.0.0.1 433 * :Nickname is already in use\r\n");
		deleteClient(clientSocket);
		return ;
	}

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
		std::cout << "ETAPE 1 CREATION NOUVEAU SALON " << std::endl << std::endl;
		std::cout << std::endl << std::endl << "OK MAINTENANT PASSONS A LA SUITE  " << std::endl << std::endl;
		msg = msg.erase(0, 6);
		Salon salon(msg);
		std::cout << "au moment de verif salon" << std::endl;
		if (verif_Salon(salon) == 0)
			std::cout << "he exit" << std::endl;
		size_t i = verif_Salon(salon);
		std::cout << std::endl << "voici le nom du salon " << _salon[i].getName() << std::endl;
		// ici faire les verifs de _opt 0 2 3
		// mettre en op si tu es le premier a etre dans channel
		if (_salon[i].get_salon_client_len() == 0)
		{
			_salon[i].setOwner(clientSocket);
			std::cout << "verif du owner " << _salon[i].getOwner() << std::endl;
		}
		else if (_salon[i].check_opt(clientSocket) == false)
			return ;
		_salon[i].increaseSocketClient(clientSocket);
		std::cout << "voici le socket " << clientSocket << std::endl;
		_salon[i].set_client(_clients, clientSocket);
		Client client = _salon[i].get_client(clientSocket);
		_salon[i].show_list_client();
		std::cout << "juste avant le send " << std::endl;
		std::string success_join = ":" + Command::clean(client.getNickname()) + "!" + Command::clean(client.getUsername()) + "@127.0.0.1 JOIN #" + Command::clean(salon.getName()) +"\r\n";
		std::cout << success_join << std::endl;
		int bytes = send(clientSocket, success_join.c_str(), success_join.size(), 0);
			if (bytes == -1)
				throw std::runtime_error("Error sending message with send");
		std::cout << std::endl << "FIN DE LA CREATION DU NOUVEAU SALON DONC FIN ETAPE 1" << std::endl << std::endl;
	}
	else if (msg.find("PING") != std::string::npos)
	{
		std::string pong = "PONG 127.0.0.1 :" + Command::clean(msg.substr(5)) + "\r\n";
		send(clientSocket, pong.c_str(), pong.size(), 0);
	}
	else if (msg.find("QUIT") != std::string::npos)
	{
		std::cout << "QUIT command" << std::endl;
	}
	else if (msg.find("KICK") != std::string::npos || msg.find("INVITE") != std::string::npos || msg.find("TOPIC") != std::string::npos || msg.find("MODE") != std::string::npos)
		Command::selectCommand(msg, this->_salon , _clients[clientSocket], this->_clients);
	else
	{
		std::cout << "je rentre dans le ELSEEEEEEEEEEEEEE DONC ETAPE 2:" << std::endl << std::endl;
		int i = 0;
		for (; (int)this->_salon.size() > i; i++)
		{
			std::cout << "salon numero " << i << " voici ses infos ====== :" << std::endl;
			std::cout << _salon[i].getName() << " : "<< std::endl;
			_salon[i].show_list_client();
		}
		if (_salon.size() > 0)
		{
			std::cout << "Voici le salon qui va envoyer un message " << std::endl;
			int nb_salon = search_salon_by_socket(clientSocket);
			std::cout << "voici le numero du salon recup " << nb_salon << std::endl;
			if (nb_salon != -1)
			{
				std::cout << _salon[search_salon_by_socket(clientSocket)].getName() << std::endl;
				std::cout << "voici la liste client dans le salon qui est selectionner " << std::endl;
				_salon[search_salon_by_socket(clientSocket)].show_list_client();
				msg_client(clientSocket, _salon[search_salon_by_socket(clientSocket)], msg);
			}
			std::cout << " no salon found with this socket" << std::endl;
		}
	}
}

void Server::msg_client(int clientSocket, Salon &tab, std::string msg)
{
	std::string envoyeur;
	std::string final;
	int i = 0;

	// std::cout << std::endl << "==== INFO POUR COMMENT LE MSG VA ETRE ENVOYE ====" << std::endl << std::endl;
	// std::cout << "ENVOIS DE LA PART DE " << clientSocket << " ET VOICI LE MESSAGE " << std::endl;
	msg = msg.erase(msg.size() - 1);
	envoyeur = tab.getName();
	int pos = msg.find(":");
	if (pos > 2147483647 || pos < 0)
		return;
	Client client = tab.get_client(clientSocket);
	std::cout << "client recup " << client.getNickname() << " socket " << clientSocket;
	std::cout << " et " << client.getSocket() << std::endl;
	tab.show_list_client();
	std::string nv = ":";
	nv.append(client.getNickname());
	std::cout << "voici le name de celui qui envois " << client.getNickname() << std::endl;
	std::cout << "et voici son socket " << client.getSocket() << std::endl;
	nv.append(" " + msg);
	nv.append("\r\n");
	while (tab.get_salon_client_len() > i)
	{
		std::cout << std::endl << std::endl;
		try
		{
				while (tab.get_SocketClient(i) > 0)
				{
					if (tab.get_SocketClient(i) != clientSocket)
					{
						std::cout << tab.get_SocketClient(i) << std::endl;
						int bytes = send(tab.get_SocketClient(i), nv.c_str(), nv.size(), 0);
						if (bytes == -1)
							throw std::runtime_error("Error sending message with send");
					}
					i++;
					if (i >= tab.get_salon_client_len())
						break;
				}
		}
		catch(std::exception &e)
		{
			throw std::runtime_error("a problem happend when sending message");
		}
		std::cout << "==== FIN DES INFOS POUR COMMENT LE MSG VA ETRE ENVOYER ====" << std::endl << std::endl;
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
