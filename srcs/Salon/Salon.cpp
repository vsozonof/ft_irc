#include "Salon.hpp"

Salon::Salon() {}

Salon::Salon(const Salon &src)
{
    _Name = src._Name;
    _SocketClient = src._SocketClient;
    std::cout << "New salon created" << std::endl;
}

Salon& Salon::operator=(Salon const &rhs)
{
    if (this != &rhs)
    {
        _Name = rhs._Name;
        _SocketClient = rhs._SocketClient;
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
    int i = 0;
    std::cout << "donc maintenant je suis dans get_client pour obtenir le client repondant au socket";
    std::cout << std::endl;
    std::cout << "voici mon socket " << ClientSocket;
    std::cout << " et voici ce qu'il y a dans l'emplacement du socket ";
    int j = 0;
    std::cout << "parti test client" << std::endl << std::endl;
    std::cout << "size " << clients.size() << std::endl;
    std::cout << "peut etre juste au dessus " << std::endl;
    while (j < 10)
    {
        std::cout << std::endl;
        if (clients[i].getSocket() == ClientSocket)
        break;
        i++;
        j++;
    }
    // while (ClientSocket != this->clients[i].getSocket())
    std::cout << "fin boucle : Client_socket " << ClientSocket;
    std::cout << " La ou je suis " << i;
    std::cout << " et ce que j'ai " << clients[i].getSocket();
    return clients[i];
}

void Salon::set_client(std::map<int, Client>& client, int clientSocket)
{
    std::cout << "debut set_client" << std::endl;
	std::cout << "nick " << client[clientSocket].getNickname();
	std::cout << " user " << client[clientSocket].getUsername();
	std::pair <int, Client> clientpair(clientSocket, client[clientSocket]);
	// if (_clients[clientSocket] )
	_clients.insert(clientpair);
    // this->_clients[i] = client;
	if (client.find(clientSocket) != client.end())
		std::cout << " ca existe" << std::endl;
	else
		std::cout << "Client avec ce socket n'existe pas!" << std::endl;
	std::cout << "nick " << _clients[clientSocket].getNickname();
	std::cout << " user " << _clients[clientSocket].getUsername();
	std::cout << std::endl;
}

void Salon::show_list_client()
{
    std::cout << "here is clients list " << std::endl;
    int len = this->_clients.size();
    std::cout << len << std::endl;
    std::cout << getName() << std::endl;
	while (len > 0)
    {
        std::cout << "nick " << this->_clients[len].getNickname();
        std::cout << " user " << this->_clients[len].getUsername();
        std::cout << "cc " << this->_SocketClient[0] << std::endl;
        len--;
    }
}

int Salon::get_salon_client_len()
{
    return this->_SocketClient.size();
}

int Salon::get_SocketClient(int pos)
{
	return _SocketClient[pos];
}
