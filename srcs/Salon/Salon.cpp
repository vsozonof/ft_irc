#include "Salon.hpp"

Salon::Salon() {}

Salon::Salon(const Salon &src)
{
    _Name = src._Name;
    _SocketClient = src._SocketClient;
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
}

void showMessage()
{
    // faire circuler le message
    std::cout << "je fais circuler le message" << std::endl;
}

// get_client doit pouvoir parcourir la base de donnees des clients connecter dans le salon
// et renvoyer le nom qui client qui possede le ClientSocket passer en parametre
Client Salon::get_client(int ClientSocket)
{
    int i = 0;
    while (ClientSocket != this->_clients[i].getSocket())
        i++;
    
    return _clients[i];
}

void Salon::set_client(Client client)
{
    int i = _clients.size();
    this->_clients[i] = client;
}

void Salon::show_list_client()
{
    std::cout << "here is clients list " << std::endl;
    int len = this->_clients.size();
    while (len > 0)
    {
        std::cout << this->_clients[len].getNickname();
        std::cout << this->_clients[len].getUsername();
        len--;
    }
}
