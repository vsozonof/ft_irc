/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   registrationUtils.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:27:37 by vsozonof          #+#    #+#             */
/*   Updated: 2025/05/15 16:55:55 by vsozonof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


// ! Checks Password validity
int	Server::checkPassword(std::string clientPassword)
{
	if (clientPassword == _password)
		return (0);
	else
		return (1);
}

// ! Checks if username is already in use
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

int	Server::gatherInfos(int clientSocket, std::string & msg) {
	int timeout = 0;
	
	while (42)
	{
		msg += _clients[clientSocket].receiveMsg();
		if (msg.find("USER") != std::string::npos
			&& msg.find("NICK") != std::string::npos
			&& msg.find("PASS") != std::string::npos)
		{
			std::cout << '[' << clientSocket << ']' << ": All informations successfully collected" << std::endl;
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

int Server::authClient(std::string msg, int clientSocket) {
	std::string userPass = msg.substr((msg.find("PASS") + 5), _password.length());
	std::string userNick = extractValue(msg, "NICK");
	std::string userName = extractValue(msg, "USER");

	std::cout << "______________________________________" << std::endl;
	std::cout << "\nID [" << clientSocket << "]: " << "INFOS RECEIVED:"
				<< "\nPASS: " << '[' << userPass << ']'
				<< "\nNICK: " << '[' << userNick << ']'
				<< "\nNAME: " << '[' << userName << ']';
	std::cout << "\n------------------------------------" << '\n';

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

	_clients[clientSocket].setNickname(userNick);
	_clients[clientSocket].setUsername(userName);

	return (0);
}

// ! Extracts KEY in MSG -> will extract the pass/nick/user from IRSSI's msg
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