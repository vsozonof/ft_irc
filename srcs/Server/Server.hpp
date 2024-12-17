/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:03:09 by vsozonof          #+#    #+#             */
/*   Updated: 2024/11/12 15:22:29 by vsozonof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "../../includes/inc.hpp"
#include "../Client/Client.hpp"
#include "../Salon/Salon.hpp"

class Client;
class Salon;

class Server
{
	private:
		unsigned int					_port;			// Port du serveur
		std::string						_password;		// Mot de passe du serveur
		int								_socket;		// Socket du serveur
		std::map<int, Client>			_clients;		// Liste des clients connectés
		std::vector<pollfd>				_fds;			// Liste des fds des clients connectés
		std::vector<Salon>				_salon;			// Liste des salons cree
		// std::vector<Salon>::iterator	_salonList;		// iterateur pour les salons crees

	public:
		Server(unsigned int port, std::string password);
		~Server();
		
		void initServer();
		void run();
		void handleClient();

		// ? Client Regisration
		void setupNewClient(int clientSocket);
		int checkUserInfos(std::string clientPassword, std::string clientNick);
		int checkPassword(std::string clientPassword);
		int checkNick(std::string clientNick);

		
		void doClientAction(int clientSocket);

		void sendWelcome(int clientSocket);
		void broadcastMessage(std::string const &message, int clientSocket);

		void setSalon(Salon salon, int i);
		std::vector<Salon> getSalon(void);

};

#endif