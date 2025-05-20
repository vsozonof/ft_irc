#include "Salon.hpp"

Salon::Salon() {
}

Salon::Salon(const Salon &src)
{
    _Name = src._Name;
    _SocketClient = src._SocketClient;
	_message = src._message;
	_clients = src._clients;
    _opt[0] = src._opt[0];
    _opt[1] = src._opt[1];
    _opt[2] = src._opt[2];
    _opt[3] = src._opt[3];
    _owner = src._owner;
    _operator = src._operator;
}

Salon& Salon::operator=(Salon const &rhs)
{
    if (this != &rhs)
    {
        _Name = rhs._Name;
        _SocketClient = rhs._SocketClient;
		_message = rhs._message;
		_clients = rhs._clients;
        _opt[0] = rhs._opt[0];
        _opt[1] = rhs._opt[1];
        _opt[2] = rhs._opt[2];
        _opt[3] = rhs._opt[3];
        _owner = rhs._owner;
        _operator = rhs._operator;
    }
    return *this;
}

Salon::~Salon() {}

Salon::Salon(std::string name)
{
	std::map<int, Client> _clients;
	_clients = std::map<int, Client>();
    _Name = name;
    std::cout << "test" << std::endl;
    _opt[0] = false;
    _opt[1] = false;
    _opt[2] = false;
    _opt[3] = false;
    _client_limits = 0;
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


void Salon::set_invite(int socket)
{
    _invite.push_back(socket);
}
void Salon::remove_invite(int socket)
{
    std::list<int>::iterator i;
    i = _invite.begin();
    while (i != _invite.end())
    {
        if (*i == socket)
        {
            _invite.erase(i);
            break;
        }
        i++;
    }
}

bool Salon::is_invite(int socket)
{
    std::list<int>::iterator i;
    i = _invite.begin();
    while (i != _invite.end())
    {
        if (*i == socket)
        return true;
        i++;
    }
    return false;
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

Client Salon::get_client(int ClientSocket)
{
    size_t i = 0;
    while (i < _clients.size())
    {
        if (_clients[i].getSocket() == ClientSocket)
            return _clients[i];
        i++;
    }
    return -1;
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
}

void Salon::show_list_client()
{
	int i = 0;
    std::cout << "\033[31m" << "here is clients list " <<  "\033[0m" << std::endl;
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
   for (int i = 0; (int)this->_SocketClient.size() > i; i++)
    {
        if (_SocketClient[i] == socket)
        {
            _SocketClient.erase(_SocketClient.begin() + i);
            return;
        }
    }
}

void Salon::send_to_all(std::string msg)
{
    std::map<int, Client>::iterator it;
    for (size_t i = 0; this->_SocketClient.size() > i; i++)
    {
        ssize_t bytes_sent = send(_SocketClient[i], msg.c_str(), msg.length(), 0);
        if (bytes_sent == -1)
            std::cerr << "Failed to send to client fd " << it->first << ": " << strerror(errno) << std::endl;
    }
}

int Salon::getOwner()
{
    return this->_owner;
}

void Salon::setOwner(int clientSocket)
{
    this->_owner = clientSocket;
}

bool Salon::check_opt(int clientsocket)
{
	this->print_opt();
	if (_opt[0] == true) // /mode +i (met le chan sur invitation)
	{
        std::cout << "passage dans le opt[0]" << std::endl;
		std::cout << "is invite " << is_invite(clientsocket) << std::endl;
		if (is_invite(clientsocket) == false)
        {
            Client client = get_client(clientsocket);
            std::string error  = ":127.0.0.1 473 " + client.getNickname() + " #" + getName() + " :Cannot join channel (+i)\r\n";
            std::cout << "voici error" << std::endl << error << std::endl;
            send(clientsocket, error.c_str(), error.size(), 0);
            this->show_list_client();
            return false;
        }
	}
	if (_opt[2] == true) // +k
	{
        std::cout << "passage dans le opt[2]" << std::endl;
		std::string pass;
		std::cin >> pass;
		std::cout << "voici pass " << pass << std::endl;
		if (pass != get_password())
			return false;
	}
	if (_opt[3] == true) // +l 1
	{
        std::cout << "passage dans le opt[3]" << std::endl;
		std::cout << "ok la il faut test " << get_client_limits();
		std::cout << " et " << get_salon_client_len() << std::endl;
		if (get_client_limits() < get_salon_client_len())
			return false;
	}
	return true;
}

int Salon::_operator_size()
{
    return _operator.size();
}

void Salon::set_mode(bool value, int mode)
{
    _opt[mode] = value;
}

bool Salon::get_mode(int mode)
{
    return _opt[mode];
}

std::vector<Salon> Server::getSalon(void)
{
	return _salon;
}

int Server::search_salon_by_socket(int clientSocket)
{
	size_t i = 0;
	while (_salon.size() > i)
	{
		size_t j = 0;
		std::map<int, Client> client = _salon[i].get_all_client();
		while (client.size() > j)
		{
			if (client[j].getSocket() == clientSocket)
			return i;
			j++;
		}
		i++;
	}
	return -1;
}

void Salon::print_opt()
{
    std::cout << "opt[0] ";
    if (_opt[0] == true)
        std::cout << "true" << std::endl;
    else
        std::cout << "false" << std::endl;
    std::cout << "opt[1] ";
    if (_opt[1] == true)
        std::cout << "true" << std::endl;
    else
        std::cout << "false" << std::endl;
    std::cout << "opt[2] ";
    if (_opt[2] == true)
        std::cout << "true" << std::endl;
    else
        std::cout << "false" << std::endl;
    std::cout << "opt[3] ";
    if (_opt[3] == true)
        std::cout << "true" << std::endl;
    else
        std::cout << "false" << std::endl;

}

int Salon::socketSize()
{
    return _SocketClient.size();
}
