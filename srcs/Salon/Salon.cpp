#include "Salon.hpp"

Salon::Salon() {}

Salon::Salon(const Salon &src)
{
    _Name = src._Name;
    _SocketClient = src._SocketClient;
    _verif = src._verif;
}

Salon& Salon::operator=(Salon const &rhs)
{
    if (this != &rhs)
    {
        _Name = rhs._Name;
        _SocketClient = rhs._SocketClient;
        _verif = rhs._verif;
    }
    return *this;
}

Salon::~Salon() {}

Salon::Salon(std::string name) : _Name(name)
{
    _verif = 0;
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
    int i = 0;
    if (_verif == 0)
    {
        std::cout << "NOUVEAU TEST saddddddddddddddddddddddddddddddd";
        _verif = 1;
        std::cout << "ici" << std::endl;
        _SocketClient[0] = socket;
        return ;
    }
    std::cout << "je passe le premier test" << std::endl;
    while (_SocketClient[i])
        i++;
    _SocketClient[i] = socket;
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
