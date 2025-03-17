/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   registrationUtils.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:27:37 by vsozonof          #+#    #+#             */
/*   Updated: 2025/03/17 15:43:59 by vsozonof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int Server::checkUserInfos(std::string clientPassword, std::string clientNick)
{
	if (!checkPassword(clientPassword) && !checkNick(clientNick))
		return (0);
	else
		return (1);
}


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
	std::map<int, Client>::iterator it = _clients.begin();
	while (it != _clients.end())
	{
		if (it->second.getNickname() == clientNick)
			return (1);
		it++;
	}
	return (0);
}

std::string Server::extractValue(const std::string& msg, const std::string& key) {
    size_t start = msg.find(key);
    if (start == std::string::npos)
        return "";

    start += key.length() + 1;
    size_t end = msg.find_first_of(" \r\n", start);
	
    std::string extracted = (end != std::string::npos) 
			? msg.substr(start, end - start) : msg.substr(start);

    std::cout << "[DEBUG] Extracted " << key << ": [" << extracted << "]\n";
    return extracted;
}