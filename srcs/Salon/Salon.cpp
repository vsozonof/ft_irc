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

Salon::Salon(std::string name) : _Name(name), _SocketClient(0)
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

void Salon::increaseSocketClient(int socket)
{
    std::cout << "je rentre dans increase ghghhkghgghbnvnmghfgvbnghvm ";
    std::cout << "premier if passe" << std::endl;
    _SocketClient.push_back(socket);
}

std::vector<int> Salon::getSocketClient()
{
    return _SocketClient;
}

void showMessage()
{
    // faire circuler le message
    std::cout << "je fais circuler le message" << std::endl;
}
