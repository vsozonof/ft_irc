/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 09:59:53 by vsozonof          #+#    #+#             */
/*   Updated: 2024/08/08 09:23:01 by vsozonof         ###   ########.fr       */
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
	Client newClient(clientSocket);
	_clients.insert(std::pair<int, Client>(clientSocket, newClient));

	pollfd newClient_fd;
	newClient_fd.fd = clientSocket;
	newClient_fd.events = POLLIN;
	_fds.push_back(newClient_fd);
}

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

	if (msg.find("CAP") != std::string::npos)
	{
		_clients[clientSocket].sendMsg("CAP * LS : multi-prefix sasl\n");
		_clients[clientSocket].sendMsg("Enter password: ");
		// doClientAction(clientSocket);
	}

    if (!_clients[clientSocket].isAuth())
    {
        if (msg == _password)
        {
            _clients[clientSocket].setAuth(true);
            _clients[clientSocket].sendMsg("Password accepted!\nWelcome to Ft_IRC!\n");
        }
        else
        {
            _clients[clientSocket].sendMsg("FT_IRC: Invalid Password !\n");
			std::cout << "[" << msg << "]" << std::endl;
            _clients[clientSocket].closeClient();
            _clients.erase(clientSocket);
        }
    }
}
