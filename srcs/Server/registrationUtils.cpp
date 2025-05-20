/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   registrationUtils.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:27:37 by vsozonof          #+#    #+#             */
/*   Updated: 2025/05/20 04:08:29 by vsozonof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


// * checkPassword(std::string clientPassword)
// * This function will check if the given password matches the server password.
int	Server::checkPassword(std::string clientPassword)
{
	if (clientPassword == _password)
		return (0);
	else
		return (1);
}

// * checkNick(std::string clientNick)
// * This function will check if the given nickname is already in use.
int	Server::checkNick(std::string clientNick)
{
	(void)clientNick;
	std::map<int, Client>::iterator it = _clients.begin();
	while (it != _clients.end())
	{
		if (it->second.getNickname() == clientNick)
			return (1);
		it++;
	}
	return (0);
}

// * gatherInfos(int clientSocket, std::string & msg)
// * This function will parse the informations received from a client
// * and will timeout a client if too much time is taken to send the required infos.
int	Server::gatherInfos(int clientSocket, std::string & msg) {
	int timeout = 0;
	
	while (42)
	{
		msg += _clients[clientSocket].receiveMsg();
		if (msg.find("USER") != std::string::npos
			&& msg.find("NICK") != std::string::npos
			&& msg.find("PASS") != std::string::npos)
		{
			std::cout << "ID " << '[' << clientSocket << ']' << ": All informations successfully collected" << std::endl;
			return (0);
		}
		else
		{
			sleep(1);
			std::cout << '[' << clientSocket << ']' << ": Missing informations.." << std::endl;
			timeout++;
			if (timeout == 5)
			{
				std::cout << '[' << clientSocket << ']' << ": TimeOut, disconnecting client.." << std::endl;
				deleteClient(clientSocket);
				return (1);
			}
		}
	}
}

// * authClient(std::string msg, int clientSocket)
// * This function will check if the password is valid and if
// * the nickname is not already in use.
// * If the password is wrong, or nickname already in use, will disconnect the client.
int Server::authClient(std::string msg, int clientSocket) {
	std::string userPass = msg.substr((msg.find("PASS") + 5), _password.length());
	std::string userNick = extractValue(msg, "NICK");
	std::string userName = extractValue(msg, "USER");

	if (checkPassword(userPass))
	{
		_clients[clientSocket].sendMsg(":127.0.0.1 464 * :Password incorrect\r\n");
		deleteClient(clientSocket);
		return (1);
	}
	else if (checkNick(userName))
	{
		_clients[clientSocket].sendMsg(":127.0.0.1 433 * :Nickname is already in use\r\n");
		deleteClient(clientSocket);
		return (1);
	}

	std::cout << "ID " << '[' << clientSocket << ']' << ": Password and Nickname are valid" << std::endl;
	_clients[clientSocket].setNickname(userNick);
	_clients[clientSocket].setUsername(userName);
	std::cout << "\nID [" << clientSocket << "]: " << "SUCCESSFULLY REGISTERED:"
		<< "\nPASS: " << '[' << userPass << ']'
		<< "\nNICK: " << '[' << _clients[clientSocket].getNickname() << ']'
		<< "\nNAME: " << '[' << _clients[clientSocket].getUsername() << ']';
	std::cout << "\n______________________________________" << '\n';
	return (0);
}

// * extractValue(const std::string & msg, const std::string & key)
// * This function will search and extract "key" from msg and return it.
std::string Server::extractValue(const std::string& msg, const std::string& key) {
    size_t start = msg.find(key);
    if (start == std::string::npos)
        return "";

    start += key.length() + 1;
    size_t end = msg.find_first_of(" \r\n", start);
	
    std::string extracted = (end != std::string::npos) 
			? msg.substr(start, end - start) : msg.substr(start);
    return extracted;
}