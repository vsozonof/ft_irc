/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 09:59:53 by vsozonof          #+#    #+#             */
/*   Updated: 2024/07/29 11:24:42 by vsozonof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::acceptClient()
{
	int	clientSocket = accept(_socket, NULL, NULL);

	if (clientSocket == -1)
		throw std::runtime_error("Error accepting client");
	
	Client newClient(clientSocket);
	_clients.push_back(newClient);
	std::cout << "New client connected" << clientSocket << std::endl;
	std::string msg = newClient.receiveMsg();
	std::cout << "Received: " << msg << std::endl;
	if (msg.find("CAP LS") != std::string::npos)
	{
		newClient.sendMsg("CAP * LS :\n");
		msg = newClient.receiveMsg();
	}	
}


