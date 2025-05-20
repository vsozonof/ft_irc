/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 09:59:53 by vsozonof          #+#    #+#             */
/*   Updated: 2025/05/20 04:09:05 by vsozonof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <stdlib.h>
#include "../commands/command.hpp"

// * void handleClient()
// * This is the main function for handling clients
// * The function will mainly do two different things:
// * ==> Set-up and authenticate new clients
// * ==> Do authenticated clients actions (handling their commands, messages).
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
	{
		if (errno == EINTR && _shutdown == 1)
			return ;
		else
			throw std::runtime_error("Poll error");
	}
}

// * void deleteClient(int clientSocket)
// * This function will erase a clientSocket from both our fd and client lists.
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

// * void setupNewClient(int clientSocket)
// * This function will help authenticating new clients
// * ==> The function will handle auth messages from the client and treat them.
// * ===> The server will reject the new clients if they provide incorrect password or already in use names.
// * ==> If everything is correct, the client will be successfully connected to the IRC server.
void Server::setupNewClient(int clientSocket)
{
	std::cout << "\033[1m" << "______________________________________\n" << std::endl;
	std::cout << "Setting up new client: ID " << '[' << clientSocket << ']' << std::endl;

	Client newClient(clientSocket);
	_clients.insert(std::pair<int, Client>(clientSocket, newClient));
	pollfd newClient_fd;
	newClient_fd.fd = clientSocket;
	newClient_fd.events = POLLIN;
	newClient_fd.revents = 0;
	_fds.push_back(newClient_fd);
	fcntl(clientSocket, F_SETFL, O_NONBLOCK);

	std::string msg;

	if (gatherInfos(clientSocket, msg))
		return ;

	if (authClient(msg, clientSocket))
		return ;

	_clients[clientSocket].sendMsg(":127.0.0.1 001 test :Welcome to the IRC Server\r\n");
}

// * void doClientAction(int clientSocket)
// * This function will treat different client-based actions such as:
// * - joining / quitting channels,
// * - sending messages,
// * - kick users
// * - etc....
void Server::doClientAction(int clientSocket)
{
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

	if (msg.find("JOIN") != std::string::npos)
	{
		join_channel(clientSocket, msg);
		size_t i = 0;
		while (i < _salon.size())
		{
			std::cout << _salon[i].getName() << std::endl;
			i++;
		}
		return ;
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
		msg_client(clientSocket, msg);
}

bool Server::join_channel(int clientSocket, std::string msg)
{
	msg = msg.erase(0, 6);
	int i = verif_Salon(msg);
	std::cout << "voici la pos de recherche salon " << i << std::endl;
	if (i > -1)
	{
		// donc le salon est existant
		std::cout << "je rentre dans la partie salon doublon" << std::endl;
		if (_salon[i].check_opt(clientSocket) == false)
		{
			std::cout << "je pars car mon opt est pas bon" << std::endl;
			std::cout << "voici celui qui pars " << clientSocket << std::endl;
			return 0;
		}
		_salon[i].increaseSocketClient(clientSocket);
		_salon[i].set_client(_clients, clientSocket);
		Client client = _salon[i].get_client(clientSocket);
		std::string success_join = ":" + Command::clean(client.getNickname()) + "!" + Command::clean(client.getUsername()) + "@127.0.0.1 JOIN #" + Command::clean(_salon[i].getName()) +"\r\n";
		int bytes = send(clientSocket, success_join.c_str(), success_join.size(), 0);
		if (bytes == -1)
			throw std::runtime_error("Error sending message with send");
	}
	else
	{
		std::cout << "je rentre dans la partie salon unique" << std::endl;
		Salon salon(msg);
		salon.increaseSocketClient(clientSocket);
		salon.set_client(_clients, clientSocket);
		Client client = salon.get_client(clientSocket);
		if (salon.get_salon_client_len() == 1)
		{
			salon.setOwner(clientSocket);
			salon.set_operator(clientSocket);
		}
		_salon.push_back(salon);
		std::cout << "salon size bien plus tot " << _salon.size();
		std::cout << "et voici le name " << _salon[0].getName() << std::endl;
		std::string success_join = ":" + Command::clean(client.getNickname()) + "!" + Command::clean(client.getUsername()) + "@127.0.0.1 JOIN #" + Command::clean(salon.getName()) +"\r\n";
		int bytes = send(clientSocket, success_join.c_str(), success_join.size(), 0);
		if (bytes == -1)
			throw std::runtime_error("Error sending message with send");
	}
	std::cout << "je passe par salon" << std::endl;
	return 1;
}

void Server::msg_client(int clientSocket, std::string msg)
{
	std::string envoyeur;
	std::string final;

	if (_salon.size() > 0)
	{
		int nb_salon = search_salon_by_socket(clientSocket);
		if (nb_salon != -1)
		{
			Salon tab = _salon[search_salon_by_socket(clientSocket)];
			msg = msg.erase(msg.size() - 1);
			envoyeur = tab.getName();
			int pos = msg.find(":");
			if (pos > 2147483647 || pos < 0)
				return;
			Client client = tab.get_client(clientSocket);
			std::string nv = ":";
			nv.append(client.getNickname());
			nv.append(" " + msg);
			nv.append("\r\n");
			send_msg_client(clientSocket, nv, tab);
		}
	}
}

void Server::send_msg_client(int clientSocket, std::string nv, Salon &tab)
{
	for (int i = 0; tab.get_salon_client_len() > i; i++)
	{
		try
		{
			while (tab.get_SocketClient(i) > 0)
			{
				if (tab.get_SocketClient(i) != clientSocket)
				{
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
	}
}
