/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   registrationUtils.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:27:37 by vsozonof          #+#    #+#             */
/*   Updated: 2024/11/12 15:29:07 by vsozonof         ###   ########.fr       */
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

int	Server::checkPassword(std::string clientPassword)
{
	if (clientPassword == _password)
		return (0);
	else
		return (1);
}

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
