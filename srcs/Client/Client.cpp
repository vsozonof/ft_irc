/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 10:45:06 by vsozonof          #+#    #+#             */
/*   Updated: 2025/05/22 16:35:38 by vsozonof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() : _socket(-1), registered(false), lastActive(time(NULL)) {}

Client::Client(int socket) : _socket(socket), registered(false), lastActive(time(NULL)) {}

Client::~Client() {}

Client::Client(const Client &src)
{
	_socket = src.getSocket();
	_nickname = src._nickname;
	_username = src._username;
	_password = src._password;
	registered = false;
	lastActive = time(NULL);
}

Client& Client::operator=(Client const &rhs)
{
	if (this != &rhs)
	{
		_socket = rhs.getSocket();
		_nickname = rhs._nickname;
		_username = rhs._username;
		_password = rhs._password;
		registered = false;
		lastActive = time(NULL);
	}
	return (*this);
}

int	Client::getSocket() const { return (_socket); }

void Client::closeClient()
{
	close(_socket);
}

void Client::setNickname(std::string const &nickname)
{
	_nickname = nickname;
}

std::string Client::getNickname() const
{
	return (_nickname);
}

void Client::setUsername(std::string const &username)
{
	_username = username;
}

std::string Client::getUsername() const
{
	return (_username);
}

void Client::sendMsg(std::string const &msg)
{
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
		return ("EMPTY MSG");
	}
	buffer[bytesRecv - 1] = '\0';
	return (std::string(buffer));
}
