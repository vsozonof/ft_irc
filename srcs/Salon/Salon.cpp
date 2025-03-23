#include "Salon.hpp"

Salon::Salon() {}

Salon::Salon(const Salon &src)
{
    std::cout << "New salon created" << std::endl;
    _Name = src._Name;
    _SocketClient = src._SocketClient;
	_message = src._message;
	_clients = src._clients;
}

Salon& Salon::operator=(Salon const &rhs)
{
    if (this != &rhs)
    {
		std::cout << "constructeur par copy" << std::endl;
        _Name = rhs._Name;
        _SocketClient = rhs._SocketClient;
		_message = rhs._message;
		_clients = rhs._clients;
    }
    return *this;
}

Salon::~Salon() {}

Salon::Salon(std::string name) : _Name(name)
{
    std::cout << "voici donc mon Name " << _Name << std::endl;
}

std::string Salon::getName()
{
    return this->_Name;
}

void Salon::setName(std::string name)
{
    this->_Name = name;
}

void Salon::setMessage(std::string msg)
{
    _message = msg;
}

std::string Salon::getMessage(void)
{
    return _message;
}

void Salon::increaseSocketClient(int socket)
{
    _SocketClient.push_back(socket);
    std::cout << "🔥voici tous mes socket 🔥" << std::endl;
    int i = _SocketClient.size();
    while (i > 0)
    {
        i--;
        std::cout << "socket " << _SocketClient[i] << std::endl;
    }
}

void Salon::showMessage()
{
    // faire circuler le message
}

// get_client doit pouvoir parcourir la base de donnees des clients connecter dans le salon
// et renvoyer le nom qui client qui possede le ClientSocket passer en parametre

Client Salon::get_client(int ClientSocket, std::map<int, Client> clients)
{
    return clients[ClientSocket];
}

void Salon::set_client(std::map<int, Client>& client, int clientSocket)
{
    std::cout << "debut set_client donc le client que je VAIS enregistrer" << std::endl;
	std::cout << "nick " << _clients[clientSocket].getNickname();
	std::cout << " user " << _clients[clientSocket].getUsername();
	std::cout << " =====" << std::endl;
	std::pair <int, Client> clientpair(clientSocket, client[clientSocket]);
	// if (_clients[clientSocket] )
	_clients.insert(clientpair);
    // this->_clients[i] = client;
	if (_clients.find(clientSocket) != client.end())
		std::cout << " ca existe" << std::endl;
	else
		std::cout << "Client avec ce socket n'existe pas!" << std::endl;
	_clients[clientSocket].setNickname(client[clientSocket].getNickname());
	_clients[clientSocket].setUsername(client[clientSocket].getUsername());
	show_client_infos(clientSocket);
	std::cout << std::endl;
}

void Salon::show_list_client()
{
	int i = 0;
    std::cout << "here is clients list " << std::endl;
    std::cout << "dans le salon " << getName() << std::endl;
	while (1)
    {
        std::cout << "nick " << this->_clients[this->_SocketClient[i]].getNickname();
        std::cout << " | user " << this->_clients[this->_SocketClient[i]].getUsername();
    	std::cout << " cc " << this->_SocketClient[0] << std::endl;
        i++;
		if (get_salon_client_len() <= i)
			break;
    }
}

void Salon::show_client_infos(int ClientSocket)
{
	std::cout << "nick [" << _clients[ClientSocket].getNickname() << "]";
	std::cout << " user [" << _clients[ClientSocket].getUsername() << "]" << std::endl;
}

int Salon::get_salon_client_len()
{
    return this->_SocketClient.size();
}

int Salon::get_SocketClient(int pos)
{
	if (pos >= 0)
        return _SocketClient[pos];
	std::cerr << "Index hors limites : " << pos << std::endl;
	return -1;
}
