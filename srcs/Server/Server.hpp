/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:03:09 by vsozonof          #+#    #+#             */
/*   Updated: 2024/08/08 08:01:47 by vsozonof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "../../includes/inc.hpp"
#include "../Client/Client.hpp"

class Client;

class Server
{
	private:
		unsigned int					_port;			// Port du serveur
		std::string						_password;		// Mot de passe du serveur
		int								_socket;		// Socket du serveur
		std::map<int, Client>			_clients;		// Liste des clients connectés
		std::vector<pollfd>				_fds;			// Liste des fds des clients connectés

	public:
		Server(unsigned int port, std::string password);
		~Server();
		
		void initServer();
		void run();
		void handleClient();
		void setupNewClient(int clientSocket);
		void doClientAction(int clientSocket);

		void sendWelcome(int clientSocket);
		void broadcastMessage(std::string const &message, int clientSocket);
		
};

#endif