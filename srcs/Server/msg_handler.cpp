/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg_handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 10:11:22 by vsozonof          #+#    #+#             */
/*   Updated: 2024/08/08 08:23:54 by vsozonof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::sendWelcome(int clientSocket)
{
	std::string welcome = "Welcome to Ft_IRC !\n";
	send(clientSocket, welcome.c_str(), welcome.size() + 1, 0);
}

void Server::broadcastMessage(std::string const &message, int clientSocket)
{
	(void)clientSocket;
	(void)message;
}