/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotilli <tpotilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 10:45:06 by vsozonof          #+#    #+#             */
/*   Updated: 2025/05/07 14:02:40 by tpotilli         ###   ########.fr       */
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
	_nickname = src._nickname;
	_username = src._username;
	_password = src._password;
}

Client& Client::operator=(Client const &rhs)
{
	if (this != &rhs)
	{
		std::cout << "je passe par la copie" << std::endl;
		_socket = rhs.getSocket();
		_authenticated = rhs.isAuth();
		_nickname = rhs._nickname;
		_username = rhs._username;
		_password = rhs._password;
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
