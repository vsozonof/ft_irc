#ifndef SALON_HPP
# define SALON_HPP

#include <iostream>
#include <string>
#include <exception>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <map>
#include <poll.h>
#include <cstdio>
#include <netdb.h>
#include <bits/stdc++.h>
#include "../Client/Client.hpp"

//classe salon
//-> doit avoir plusieurs clients qui pourront s'envoyer des messages
// le salon doit avoir un nom
// -> mettre a jours les clients quand ils se connectent

class Client;

class Salon
{
	public:
        Salon(std::string name);
		Salon(const Salon &src);
		Salon& operator=(Salon const &rhs);
		Salon();
		~Salon();

        void setName(std::string name);
		void setMessage(std::string msg);

        std::string getName();
		Client get_client(int ClientSocket, std::map<int, Client> clients);
		std::string getMessage(void);
		int get_salon_client_len();
		int get_SocketClient(int pos);
		std::map<int, Client> get_all_client();


        void showMessage();
		void show_client_infos(int ClientSocket);
		void set_client(std::map<int, Client>& client, int clientSocket);
		void increaseSocketClient(int socket);
		void show_list_client();

	private:
		std::string						_Name;				// name du Salon
		std::vector<int>				_SocketClient;		// nombre de client
		std::string						_message;
		std::map<int, Client>			*_clients;
};

#endif