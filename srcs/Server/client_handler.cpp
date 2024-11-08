/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 09:59:53 by vsozonof          #+#    #+#             */
/*   Updated: 2024/11/06 09:58:28 by vsozonof         ###   ########.fr       */
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

	// ! CAP Negociation && Password Check
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
			std::cout << "Not all infos received" << std::endl;
	}

	_clients[clientSocket].setNickname(msg.substr((msg.find("NICK") + 5), msg.find("USER") - msg.find("NICK") - 6));
	_clients[clientSocket].setUsername(msg.substr((msg.find("USER") + 5), msg.find("PASS") - msg.find("USER") - 6));
	
	if (msg.find("PASS") != std::string::npos)
	{
		std::string password = msg.substr((msg.find("PASS") + 5), _password.length());
		std::cout << "Password received: " << password << std::endl;
		if (password == _password)
			std::cout << "Password received and correct" << std::endl;
		else
			std::cout << "Password received but incorrect - TBD" << std::endl;
	}
	else
		std::cout << "Password not received - TBD" << std::endl;

	_clients[clientSocket].setAuth(true);
	_clients[clientSocket].sendMsg("CAP * LS:\r\n");
	
	msg = _clients[clientSocket].receiveMsg();
	std::cout << clientSocket << " NewReceived: " << msg << std::endl;

	if (msg.find("CAP END") != std::string::npos)
		std::cout << "CAP Negociation ended" << std::endl;
	else
		throw std::runtime_error("CAP Negociation failed");
	_clients[clientSocket].sendMsg("CAP * END\r\n");
	sleep(1);
	_clients[clientSocket].sendMsg(":localhost 001 vsozonof :Welcome to the Internet Relay Network localhost\r\n");
}

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
			std::cout << "Unknown command" << std::endl;
			_clients[clientSocket].sendMsg("Unknown command\r\n");
		}

}
