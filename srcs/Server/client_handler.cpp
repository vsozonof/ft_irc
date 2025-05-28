/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 09:59:53 by vsozonof          #+#    #+#             */
/*   Updated: 2025/05/28 08:24:25 by vsozonof         ###   ########.fr       */
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
	int ret = poll(_fds.data(), _fds.size(), 100);

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
	int i = search_salon_by_socket(clientSocket);
	if (i > -1)
		delete_user_all_salon(clientSocket);
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

	std::string welcome = ":Ft_Irc 001 " + _clients[clientSocket].getNickname() + " :Welcome to the IRC server!\r\n";
	_clients[clientSocket].sendMsg(welcome);
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
	_clients[clientSocket].lastActive = time(NULL);

    if (bytesRecv <= 0)
    {
		std::cout << "Client " << clientSocket << " disconnected" << std::endl;
        _clients[clientSocket].closeClient();
        _clients.erase(clientSocket);
        return;
    }

    buffer[bytesRecv - 1] = '\0';
    std::string msg(buffer);
	msg = "/" + msg;
	if (msg.find("/JOIN") != std::string::npos)
	{
		msg = msg.substr(msg.find_first_of("/") + 1, std::string::npos);
		join_channel(clientSocket, msg);
		return ;
	}
	if (msg.find("/PING") != std::string::npos)
	{
		msg = msg.substr(msg.find_first_of("/") + 1, std::string::npos);
		std::string pong = "PONG 127.0.0.1 :" + Command::clean(msg.substr(5)) + "\r\n";
		send(clientSocket, pong.c_str(), pong.size(), 0);
	}
	else if (msg.find("/QUIT") != std::string::npos)
	{
		msg = msg.substr(msg.find_first_of("/") + 1, std::string::npos);
		deleteClient(clientSocket);
	}
	else if (msg.find("/KICK") != std::string::npos || msg.find("/INVITE") != std::string::npos || msg.find("/TOPIC") != std::string::npos || msg.find("/MODE") != std::string::npos)
	{
		msg = msg.substr(msg.find_first_of("/") + 1, std::string::npos);
		Command::selectCommand(msg, this->_salon , _clients[clientSocket], this->_clients);
	}
	else
	{
		msg = msg.substr(msg.find_first_of("/") + 1, std::string::npos);
		msg_client(clientSocket, msg);
	}
}

bool Server::join_channel(int clientSocket, std::string buf)
{
	buf = buf.erase(0, 6);
	size_t j = 0;
	for (;j < buf.size(); j++)
	if (isspace(buf[j]))
		break;
	std::string msg = buf.substr(0, j);

	for (size_t i = 0; i < msg.size(); ++i) {
        if (!std::isalnum(static_cast<unsigned char>(msg[i])))
		{
			_clients[clientSocket].sendMsg(":yourserver 403 " + _clients[clientSocket].getNickname() + " " + msg + " :No such channel\r\n");
            return -1;		
		}
	}

	int i = verif_Salon(msg, clientSocket);
	if (i == -2)
		return 0;
	if (i > -1)
	{
		if (_salon[i].check_opt(clientSocket, search_client(clientSocket), buf) == false)
			return 0;
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
		std::string success_join = ":" + Command::clean(client.getNickname()) + "!" + Command::clean(client.getUsername()) + "@127.0.0.1 JOIN #" + Command::clean(salon.getName()) +"\r\n";
		int bytes = send(clientSocket, success_join.c_str(), success_join.size(), 0);
		if (bytes == -1)
			throw std::runtime_error("Error sending message with send");
	}
	i = verif_Salon(msg, clientSocket);
	Client client = _salon[i].get_client(clientSocket);
	std::map<int, Client> clients = _salon[i].get_all_client();
	std::string msg_join1 = ":127.0.0.1 353 ";
	msg_join1.append(client.getNickname() + " = #" + Command::clean(_salon[i].getName()) + " :");
	for(int j = 0; j < _salon[i].get_salon_client_len(); j++)
		msg_join1.append(clients[j].getNickname() + " ");
	msg_join1.append("\r\n");
	int bytes = send(clientSocket, msg_join1.c_str(), msg_join1.size(), 0);
	if (bytes == -1)
		throw std::runtime_error("Error sending message with send");
	return 1;
}

void Server::msg_client(int clientSocket, std::string msg)
{
	std::string envoyeur;
	std::string final;

	if (_salon.size() > 0)
	{
		if (msg.find("#") == std::string::npos)
		{
			if (msg.size())
			{
				std::string nick = extractValue(msg, "PRIVMSG");
				std::string message = msg.substr(msg.find(":"), msg.size() - msg.find(":") - 1);
				_clients[clientSocket].getNickname();
				for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
				{
					if (it->second.getNickname() == nick)
					{
						std::string nv = (":");
						nv.append(Command::clean(_clients[clientSocket].getNickname()));
						nv.append("!");
						nv.append(Command::clean(_clients[clientSocket].getUsername()));
						nv.append("@127.0.0.1 PRIVMSG ");
						nv.append(Command::clean(nick));
						nv.append(" ");
						nv.append(message);
						nv.append("\r\n");
						int bytes = send(it->second.getSocket(), nv.c_str(), nv.size(), 0);
						if (bytes == -1)
						throw std::runtime_error("Error sending message with send");
						return;
					}
				}
			}
			return ;
		}
		int nb_salon = search_salon_msg(msg);
		Salon tab = _salon[nb_salon];
		Client env = search_client(clientSocket);
		if (nb_salon != -1 && search_salon_socket_and_msg(clientSocket, msg) == 1)
		{
			Salon tab = _salon[nb_salon];
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

int Server::search_salon_msg(std::string msg)
{
	size_t j = 0;
	int pos = msg.find("#");
	for (int save = 0;j < msg.size(); j++)
	{
		if (isspace(msg[j]))
		{
			if (save == 1)
				break;
			save++;
		}
	}
	std::string salon_name = msg.substr(pos + 1, j - pos - 1);
	j = 0;
	while (_salon.size() > j)
	{
		if (_salon[j].getName() == salon_name)
			return j;
		j++;
		if (j == _salon.size())
			return -1;
	}
	return -1;
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

void Server::send_msg_priv(int clientSocket, std::string nv, Salon &tab)
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
