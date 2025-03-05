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
        void showMessage();
		Client get_client(int ClientSocket);
		void set_client(Client client);
		void show_list_client();
		int get_salon_client_len();
		std::string get_topic();
		void set_topic(std::string topic);
		void set_password(std::string password);
		std::string get_password();
		void set_opt(int opt, bool value);
		bool get_opt(int opt);
		void set_operator(int socket);
		void remove_operator(int socket);
		bool is_operator(int socket);
		void set_client_limits(int limits);
		int get_client_limits();



	private:
		std::string						_Name;				// name du Salon
		std::string						_Topic;				// topic du Salon
		std::vector<int>				_SocketClient;		// nombre de client
		std::string						_message;
		std::map<int, Client>			_clients;
		std::string						_password; // mot de passe du salon
		std::list<int>					_operator; // liste des opérateurs
		int								_client_limits; // nombre de client max
		int								_va_test;
		bool								_opt[4]; // 0 = invitation, 1 = mode topic, 2 = mode password, 3 = user limit
};

#endif
