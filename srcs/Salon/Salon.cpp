#include "Salon.hpp"

Salon::Salon() {}

Salon::Salon(const Salon &src)
{
    _Name = src._Name;
    _numberOfClient = src._numberOfClient;
}

Salon& Salon::operator=(Salon const &rhs)
{
    if (this != &rhs)
    {
        _Name = rhs._Name;
        _numberOfClient = rhs._numberOfClient;
    }
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

int Salon::getNumberOfClient()
{
    return this->_numberOfClient;
}

void showMessage()
{
    // faire circuler le message
    std::cout << "je fais circuler le message" << std::endl;
}
