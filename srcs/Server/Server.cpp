/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:02:02 by vsozonof          #+#    #+#             */
/*   Updated: 2025/05/22 17:30:19 by vsozonof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <ctime>

Server::Server(unsigned int port, std::string password) : _port(port), _password(password)
{
	std::cout << BOLD_GREEN << "⚙️  Ft_IRC: Creating IRC Server ..." << def << std::endl;
	initServer();
}

Server::~Server()
{
	std::cout << BOLD_GREEN << "🗑️  Ft_IRC: Server destroyed" << def << std::endl;
}

// * void initServer()
// * This function will initiate everything that is required for the server to run
// * - Creates a TCP socket for the server 
// * - Allow the socket address reuse  (to be able to restart the server quickly)
// * - Configure the server address (IP, port...)
// * - Bind the socket to the address and port and listen for connections
// * - Add the socket to the poll list
// * If everything goes well, the server is online, else an error is thrown!

void Server::initServer()
{
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1)
		throw std::runtime_error(BOLD_RED "Socket creation failure" def);

	int opt = 1;
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw std::runtime_error("setsockopt(SO_REUSEADDR) failure");

	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(_port);
	servAddr.sin_addr.s_addr = INADDR_ANY;
	
	if (bind(_socket, (sockaddr *)&servAddr, sizeof(servAddr)) == -1)
		throw std::runtime_error(BOLD_RED "Socket bind() failure" def);
	
	if (listen(_socket, 10) == -1)
		throw std::runtime_error(BOLD_RED "Socket listen() failure" def);

	pollfd listening_fd;
	listening_fd.fd = _socket;
	listening_fd.events = POLLIN;
	listening_fd.revents = 0;
	_fds.push_back(listening_fd);
	_shutdown = 0;
	std::cout << BOLD_GREEN << "⚙️  Ft_IRC: Server successfully initialized"  << std::endl;
	std::cout << "🚪 Ft_IRC: Port: " << _port
				<< "\n🗺️  Ft_IRC: Address: 127.0.0.1" 
				<< "\n🔒 Ft_IRC: Password: " << _password << def << std::endl;
}

void Server::initServerShutdown() {
	_shutdown = 1;
}

Server* g_server = NULL;

void sigHandler(int s)
{
	(void)s;
	g_server->initServerShutdown();
}

void Server::initSigHandler() {
	struct sigaction sigIntHandler;
	
	sigIntHandler.sa_handler = sigHandler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);

	struct sigaction sigPipeHandler;
	sigaction(SIGPIPE, &sigPipeHandler, NULL);
	sigPipeHandler.sa_handler = SIG_IGN;
	sigemptyset(&sigPipeHandler.sa_mask);
	sigPipeHandler.sa_flags = 0;
	
	g_server = this;
}

void Server::timeOutInactiveClients() {
	time_t now = time(NULL);
	std::map<int, Client>::iterator it = _clients.begin();
	while (it != _clients.end()) {
		Client& client = it->second;
		
		if (now - client.lastActive >= 120 && client.registered) {
			std::cout << "ID [" << client.getSocket() << ']' << " : timed out, disconnecting." << std::endl;
			++it;
			deleteClient(client.getSocket());
		}
		else
			++it;
	}
}

void Server::run()
{	
	initSigHandler();

	while (!_shutdown) {
		handleClient();
		timeOutInactiveClients();
	}
	std::cout << BOLD_GREEN << "\n⚙️  Ft_IRC: Successfully stopped the server!" << std::endl;
	close(_socket);
}

int Server::verif_Salon(std::string name, int clientSocket)
{
	int i = 0;
	(void)clientSocket;
	if (_salon.size() == 0 || _salon.size() > 2147483647)
	{
		std::cout << "je pars" << std::endl;
		return -1;
	}
	std::cout << "je passe par la" << std::endl;
	size_t tmp = _salon.size();
	std::string name2 = Command::clean(name);
	while (tmp > 0)
	{
		Command::debug_print(name2);
		Command::debug_print(_salon[i].getName());
		if (_salon[i].getName() == name2)
			return i;
		i++;
		tmp--;
	}
	return -1;
}

Client Server::search_client(int socket)
{
	for (size_t i = 0; i < _clients.size() ;i++)
	{
		if (socket == _clients[i].getSocket())
		{
			std::cout << "CLIENT RECUP " << _clients[i].getSocket() << std::endl;
			std::cout << "le name = " << _clients[i].getNickname() << std::endl;
			return (_clients[i]);
		}
	}
	return 0;
}

bool Server::is_already_in_serv(int socket)
{
	size_t j = 0;
	while (j < _salon.size())
	{
		std::map<int, Client> clients = _salon[j].get_all_client();
		size_t o = 0;
		for (int i = 0; i < _salon[j].get_salon_client_len() ;i++)
		{
			while (clients[o].getSocket() == -1)
				o++;
			std::cout << "voici le client " << clients[o].getSocket() << std::endl;
			if (socket == clients[o].getSocket())
			{
				std::cout << "je remove le client du salon " << _salon[j].getName() << std::endl;
				_salon[j].remove_client(socket);
				_salon[j].show_list_client();
				return 1;
			}
			o++;
		}
		j++;
	}
	std::cout << "he was not already in serv" << std::endl;
	return 0;
}

int Server::search_salon_by_socket(int clientSocket)
{
	size_t i = 0;
	while (_salon.size() > i)
	{
		size_t j = 0;
		std::map<int, Client> client = _salon[i].get_all_client();
		while (client.size() > j)
		{
			if (client[j].getSocket() == clientSocket)
			return i;
			j++;
		}
		i++;
	}
	return -1;
}
