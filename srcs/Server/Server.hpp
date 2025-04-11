/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotilli <tpotilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:03:09 by vsozonof          #+#    #+#             */
/*   Updated: 2025/03/26 16:28:53 by tpotilli         ###   ########.fr       */
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
		std::string extractValue(const std::string& msg, const std::string& key);

		
		void doClientAction(int clientSocket);

		void sendWelcome(int clientSocket);
		void broadcastMessage(std::string const &message, int clientSocket);

		int setSalon(Salon salon, int i);
		int verif_Salon(Salon salon);
		std::vector<Salon> getSalon(void);

		void msg_client(int clientSocket, Salon &tab, std::string msg);

};

#endif