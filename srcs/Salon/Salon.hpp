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
		Client get_client(int ClientSocket);
		std::string getMessage(void);
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
		void set_invite(int socket);
		void remove_invite(int socket);
		bool is_invite(int socket);
		void set_client_limits(int limits);
		int get_client_limits();
		int get_SocketClient(int pos);
		std::map<int, Client> get_all_client();
		void remove_client(int socket);
		void send_to_all(std::string msg);

        void showMessage();
		void show_client_infos(int ClientSocket);
		void set_client(std::map<int, Client>& client, int clientSocket);
		void increaseSocketClient(int socket);
		void show_list_client();

	private:
		std::string						_Name;				// name du Salon
		std::string						_Topic;				// topic du Salon
		std::vector<int>				_SocketClient;		// nombre de client
		std::string						_message;
		std::map<int, Client>			_clients;
		std::string						_password; // mot de passe du salon
		std::list<int>					_operator; // liste des opérateurs
		std::list<int>					_invite; // liste des invites
		int								_client_limits; // nombre de client max
		bool								_opt[4]; // 0 = invitation, 1 = mode topic, 2 = mode password, 3 = user limit
};

#endif
