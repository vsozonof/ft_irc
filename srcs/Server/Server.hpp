/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:03:09 by vsozonof          #+#    #+#             */
/*   Updated: 2025/05/22 16:40:52 by vsozonof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "../../includes/inc.hpp"
#include "../Client/Client.hpp"
#include "../Salon/Salon.hpp"
#include "../commands/command.hpp"
#include <fcntl.h>

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
		int								_shutdown;

	public:
		Server(unsigned int port, std::string password);
		~Server();

		void initServer();
		void initSigHandler();
		void run();
		void handleClient();
		void timeOutInactiveClients();

		// ? Client Regisration
		void setupNewClient(int clientSocket);
		void deleteClient(int clientSocket);
		int	gatherInfos(int clientSocket, std::string & msg);
		int authClient(std::string msg, int clientSocket);
		int checkPassword(std::string clientPassword);
		int checkNick(std::string clientNick);
		std::string extractValue(const std::string& msg, const std::string& key);
		void	initServerShutdown();


		void doClientAction(int clientSocket);

		int setSalon(Salon salon, int i);
		int verif_Salon(std::string name);
		std::vector<Salon> getSalon(void);
		int search_salon_by_socket(int clientSocket);

		void msg_client(int clientSocket, std::string msg);
		Client search_client(int socket);

		bool join_channel(int clientSocket, std::string msg);
		void send_msg_client(int clientSocket, std::string nv, Salon &tab);
};

#endif