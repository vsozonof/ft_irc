/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 10:45:06 by vsozonof          #+#    #+#             */
/*   Updated: 2024/07/29 10:54:30 by vsozonof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int socket) : _socket(socket), _authenticated(false) {}

Client::~Client() { closeClient(); }

int	Client::getSocket() const { return (_socket); }

void Client::setAuth(bool auth) { _authenticated = auth; }

int Client::isAuth() const { return (_authenticated); }

void Client::closeClient()
{
	close(_socket);
}

void Client::sendMsg(std::string const &msg)
{
	send(_socket, msg.c_str(), msg.size() + 1, 0);
}

std::string Client::receiveMsg()
{
	char buffer[1024];
	int bytesRecv = recv(_socket, buffer, sizeof(buffer), 0);
	
	if (bytesRecv <= 0)
	{
		close(_socket);
		return ("");
	}
	
	buffer[bytesRecv - 1] = '\0';
	return (std::string(buffer));
}
