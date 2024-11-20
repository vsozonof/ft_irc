/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 10:45:06 by vsozonof          #+#    #+#             */
/*   Updated: 2024/09/08 16:31:24 by vsozonof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() : _socket(-1), _authenticated(false) {}

Client::Client(int socket) : _socket(socket), _authenticated(false) {}

Client::~Client() {}

Client::Client(const Client &src)
{
	_socket = src.getSocket();
	_authenticated = src.isAuth();
}

Client& Client::operator=(Client const &rhs)
{
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
	std::cout << _socket << " Sending: " << msg << std::endl;
	// surement ici pour transmettre le message pour plusieurs client
	int bytes = send(_socket, msg.c_str(), msg.size() + 1, 0);
	if (bytes == -1)
		throw std::runtime_error("Error sending message");
}

std::string Client::receiveMsg()
{
	char buffer[1024];
	int bytesRecv = recv(_socket, buffer, sizeof(buffer), 0);
	// surement ici pour recuperer le message pour plusieurs client
	if (bytesRecv <= 0)
	{
		close(_socket);
		return ("EMPTY MSG");
	}
	// std::cout << "je viens de recevoir un message";
	buffer[bytesRecv - 1] = '\0';
	return (std::string(buffer));
}
