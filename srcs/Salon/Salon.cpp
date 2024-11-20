#include "Salon.hpp"

Salon::Salon();
{

}

Salon(const Salon &src);
Salon& operator=(Salon const &rhs);
~Salon();

Salon::Salon(std::string name) : _Name(name) {}

std::string Salon::getName()
{
    return this->_Name;
}

void Salon::setName(std::string name);
{

}

showMessage();
