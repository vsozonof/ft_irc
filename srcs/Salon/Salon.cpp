#include "Salon.hpp"

Salon::Salon() {}

Salon::Salon(const Salon &src)
{
    _Name = src._Name;
    _SocketClient = src._SocketClient;
	_message = src._message;
	std::map<int, Client> _clients;
	_clients = std::map<int, Client>();
}

Salon& Salon::operator=(Salon const &rhs)
{
    if (this != &rhs)
    {
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
	std::map<int, Client> _clients;
	_clients = std::map<int, Client>();
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
    int i = _SocketClient.size();
    while (i > 0)
    {
        i--;
    }
}

void Salon::showMessage()
{
    // faire circuler le message
}


Client Salon::get_client(int ClientSocket)
{
    return _clients[ClientSocket];
}

std::map<int, Client> Salon::get_all_client()
{

    return this->_clients;
}

void Salon::set_client(std::map<int, Client>& client, int clientSocket)
{
	if (_clients.size() == 0)
		std::map<int, Client> _clients;
	std::pair <int, Client> clientpair(clientSocket, client[clientSocket]);
	_clients.insert(clientpair);

	_clients[clientSocket].setNickname(client[clientSocket].getNickname());
	_clients[clientSocket].setUsername(client[clientSocket].getUsername());
	show_client_infos(clientSocket);
}

void Salon::show_list_client()
{
	int i = 0;
    std::cout << "\033[31m" << "here is clients list " <<  "\033[0m" << std::endl;
	//surement dus a la dif entre mon socket et la pos des infos genre nom
	while (1)
	{
		std::cout << "voici tous les socket " << this->_SocketClient[i] << std::endl;
		i++;
		if (get_salon_client_len() <= i)
			break;
	}
	i = 0;

	while (1)
    {
        std::cout << "nick " << _clients[this->_SocketClient[i]].getNickname();
        std::cout << " | user " << _clients[this->_SocketClient[i]].getUsername();
    	std::cout << " cc " << this->_SocketClient[i] << std::endl;
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
	return -1;
}

void Salon::remove_client(int socket)
{
   _clients.erase(socket);
}
