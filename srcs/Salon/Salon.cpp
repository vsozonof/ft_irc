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

std::string Salon::get_topic()
{
    return _Topic;
}

void Salon::set_topic(std::string topic)
{
    _Topic = topic;
}

void Salon::set_password(std::string password)
{
    _password = password;
}

std::string Salon::get_password()
{
    return _password;
}

void Salon::set_opt(int opt, bool value)
{
    _opt[opt] = value;
}

bool Salon::get_opt(int opt)
{
    return _opt[opt];
}

void Salon::set_operator(int socket)
{
    _operator.push_back(socket);
}

void Salon::remove_operator(int socket)
{
    std::list<int>::iterator i;
    i = _operator.begin();
    while (i != _operator.end())
    {
        if (*i == socket)
        {
            _operator.erase(i);
            break;
        }
        i++;
    }
}

bool Salon::is_operator(int socket)
{
    std::list<int>::iterator i;
    i = _operator.begin();
    while (i != _operator.end())
    {
        if (*i == socket)
            return true;
        i++;
    }
    return false;
}

void Salon::set_client_limits(int limits)
{
    _client_limits = limits;
}

int Salon::get_client_limits()
{
    return _client_limits;
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
    std::cout << "je fais circuler le message du saint va_test " << _va_test << std::endl;
}

// get_client doit pouvoir parcourir la base de donnees des clients connecter dans le salon
// et renvoyer le nom qui client qui possede le ClientSocket passer en parametre
Client Salon::get_client(int ClientSocket)
{
    int i = 0;
    std::cout << "donc maintenant je suis dans get_client pour obtenir le client repondant au socket";
    std::cout << std::endl;
    std::cout << "voici mon socket " << ClientSocket;
    std::cout << " et voici ce qu'il y a dans l'emplacement du socket ";
    std::cout << this->_clients[i].getSocket() << std::endl;
    int j = 0;
    std::cout << "parti test client" << std::endl << std::endl;
    std::cout << "size " << _clients.size() << std::endl;
    std::cout << "test vite fait" << _clients[i].getNickname() << std::endl;
    std::cout << "voici va_test ";
    std::cout << _va_test;
    while (j < 10)
    {
        std::cout << "debut boucle : Client_socket " << ClientSocket;
        std::cout << " La ou je suis " << i;
        std::cout << " et ce que j'ai " << this->_clients[i].getSocket();
        std::cout << std::endl;
        if (this->_clients[i].getSocket() == ClientSocket)
            break;
        i++;
        j++;
    }
    // while (ClientSocket != this->_clients[i].getSocket())
    return _clients[i];
}

void Salon::set_client(Client client)
{
    std::cout << "debut set_client" << std::endl;
    int i = _clients.size();
    std::cout << "je suis dans set_client ";
    this->_clients[i] = client;
    std::cout << "i add a client at emplacement " << i << std::endl;
    std::cout << "try " << this->_clients[i].getNickname() << std::endl;
    _va_test = 1;
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

int Salon::get_salon_client_len()
{
    return this->_SocketClient.size();
}
