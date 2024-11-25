#include "Salon.hpp"

Salon::Salon() {}

Salon::Salon(const Salon &src)
{
    _Name = src._Name;
}

Salon& Salon::operator=(Salon const &rhs)
{
    if (this != &rhs)
        _Name = rhs._Name;
    return *this;
}

Salon::~Salon() {}

Salon::Salon(std::string name) : _Name(name) {}

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
    std::cout << "je rentre dans increase" << std::endl;
    int i = 0;
    while (_SocketClient[i])
    {
        std::cout << "voici les socket << " << _SocketClient  <<std::endl;
        i++;
    }
    _SocketClient[i] = socket;
    std::cout << "voici les socket << " << _SocketClient  <<std::endl;
}

int *Salon::getSocketClient()
{
    return _SocketClient;
}

void showMessage()
{
    // faire circuler le message
    std::cout << "je fais circuler le message" << std::endl;
}
