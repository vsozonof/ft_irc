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

        std::string getName();
        void setName(std::string name);
		void setMessage(std::string msg);
		std::string getMessage(void);
		void increaseSocketClient(int socket);
		std::vector<int> getSocketClient();
        void showMessage();
		std::vector<Client> get_client(int ClientSocket);
		void set_client(Client client);


	private:
		std::string						_Name;				// name du Salon
		std::vector<int>				_SocketClient;		// nombre de client
		std::string						_message;
		std::vector<Client>							_client;
};

#endif