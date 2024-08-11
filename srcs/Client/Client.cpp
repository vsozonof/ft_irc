/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 10:45:06 by vsozonof          #+#    #+#             */
/*   Updated: 2024/07/31 09:27:29 by vsozonof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() : _socket(-1), _authenticated(false) {}

Client::Client(int socket) : _socket(socket), _authenticated(false) {}

Client::~Client()
{
	std::cout << "Client " << _socket << " disconnected [no]" << std::endl;
}

Client::Client(const Client &src)
{
	std::cout << "Copy constructor called" << std::endl;
	_socket = src.getSocket();
	_authenticated = src.isAuth();
}

Client& Client::operator=(Client const &rhs)
{
	std::cout << "Assignation operator called" << std::endl;
	if (this != &rhs)
	{
		_socket = rhs.getSocket();
		_authenticated = rhs.isAuth();
	}
	return (*this);
}

int	Client::getSocket() const { return (_socket); }

void Client::setAuth(bool auth) { _authenticated = auth; }

int Client::isAuth() const { return (_authenticated); }

void Client::closeClient()
{
	close(_socket);
}

void Client::sendMsg(std::string const &msg)
{
	std::cout << _socket << "Sending: " << msg << std::endl;
	int bytes = send(_socket, msg.c_str(), msg.size() + 1, 0);
	if (bytes == -1)
		throw std::runtime_error("Error sending message");
}

std::string Client::receiveMsg()
{
	char buffer[1024];
	int bytesRecv = recv(_socket, buffer, sizeof(buffer), 0);
	
	if (bytesRecv <= 0)
	{
		close(_socket);
		return ("EMPTY MSG");
	}
	
	buffer[bytesRecv - 1] = '\0';
	return (std::string(buffer));
}
