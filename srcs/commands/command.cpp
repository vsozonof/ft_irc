/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 10:48:14 by rostrub           #+#    #+#             */
/*   Updated: 2025/05/22 16:45:38 by vsozonof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"

Command::Command()
{
}

Command::~Command()
{
}

void Command::debug_print(const std::string& msg)
{
    for (size_t i = 0; i < msg.length(); ++i)
    {
        if (msg[i] == '\r')
            std::cout << "\\r";
        else if (msg[i] == '\n')
            std::cout << "\\n";
		else if (msg[i] == ' ')
			std::cout << "sp";
        else
            std::cout << msg[i];
    }
    std::cout << std::endl;
}

std::string Command::clean(std::string s) {
    for (size_t i = 0; i < s.length(); ++i) {
        if (s[i] == '\r' || s[i] == '\n')
            s.erase(i--, 1);
    }
    return s;
}

void Command::selectCommand(std::string command, std::vector<Salon> &_salon, Client client, std::map<int, Client> &clients)
{
	if (command.find("KICK") != std::string::npos)
	{
		Salon *salon;
		for (size_t i = 0; i < _salon.size(); ++i)
		{
			size_t start = command.find("#") + 1;
			size_t end = command.find(" ", start);
			if (clean(_salon[i].getName()) == command.substr(start, end - start))
			{
				salon = &_salon[i];
				kick(command, *salon, client);
				return;
			}
		}
		size_t start = command.find("#") + 1;
		size_t end = command.find(" ", start);
		std::string channel = command.substr(start, end - start);
		std::string error = ":127.0.0.1 403 " + client.getNickname() + " #" + channel + " :No such channel\r\n";
		send(client.getSocket(), error.c_str(), error.size(), 0);
		return;
	}
	else if (command.find("INVITE") != std::string::npos)
	{
		Salon *salon = NULL;
		size_t start = command.find("#") + 1;
		size_t end = command.find("\r", start);
		for (size_t i = 0; i < _salon.size(); ++i)
		{
			if (clean(_salon[i].getName()) == command.substr(start, end - start))
			{
				salon = &_salon[i];
				invite(command, _salon[i], clients, client);
				return;
			}
		}
		if (salon == NULL)
		{
			std::string error = ":127.0.0.1 442 " + client.getNickname() + " #" + command.substr(start, end - start) + " :You're not on that channel\r\n";
			send(client.getSocket(), error.c_str(), error.size(), 0);
			return;
		}
	}
	else if (command.find("TOPIC") != std::string::npos)
	{
		Salon *salon = NULL;

		for (size_t i = 0; i < _salon.size(); ++i)
		{
			size_t start = command.find("#") + 1;
			size_t end = command.find(" ", start);
			if (clean(_salon[i].getName()) == command.substr(start, end - start))
			{
				salon = &_salon[i];
				topic(command, *salon, client);
				return;
			}
		}
		if (salon == NULL)
		{
			std::string error = ":127.0.0.1 403 " + client.getNickname() + " #" + command.substr(command.find("#") + 1, command.find(" ", command.find("#") + 1) - command.find("#") - 1) + " :No such channel\r\n";
			send(client.getSocket(), error.c_str(), error.size(), 0);
		}
		return;
	}
	else if (command.find("MODE") != std::string::npos)
	{
		Salon *salon = NULL;
		for (size_t i = 0; i < _salon.size(); ++i)
		{
			size_t start = command.find("#") + 1;
			size_t end = command.find(" ", start);
			if (clean(_salon[i].getName()) == command.substr(start, end - start))
			{
				salon = &_salon[i];
				mode(command, *salon, client);
				return;
			}
		}
		if (salon == NULL)
		{
			std::string error = ":127.0.0.1 403 " + client.getNickname() + " #" + command.substr(command.find("#") + 1, command.find(" ", command.find("#") + 1) - command.find("#") - 1) + " :No such channel\r\n";
			send(client.getSocket(), error.c_str(), error.size(), 0);
			return;
		}
	}
}

void Command::kick(std::string command, Salon &salon, Client kicker)
{
	Client client;
	bool is_in = false;
	bool is_op = false;
	for (int i = 0; i < salon.socketSize(); i++)
	{
		int socket = salon.get_SocketClient(i);
		client = salon.get_client(socket);
		if (client.getNickname() == kicker.getNickname())
		{
			is_in = true;
			break;
		}
	}
	if (is_in == false)
	{
		std::string error = ":127.0.0.1 442 " + kicker.getNickname() + " #" + clean(salon.getName()) + " :You're not on that channel\r\n";
		send(kicker.getSocket(), error.c_str(), error.size(), 0);
		return;
	}
	std::map<int, Client> clients = salon.get_all_client();
	is_op = salon.is_operator(kicker.getSocket());
	if (is_op == false)
	{
		std::string error = ":127.0.0.1 482 " + kicker.getNickname() + " #" + clean(salon.getName()) + " :You're not channel operator\r\n";
		send(kicker.getSocket(), error.c_str(), error.size(), 0);
		return;
	}
	size_t start = command.find("#") + 2 + salon.getName().size();
	size_t end = command.find(" ", start);
	std::string username = command.substr(start, end - start);
	start = command.find(":");
	end = command.find("\r", start);
	std::string raison = command.substr(start + 1, end - start - 1);
	if (raison.empty())
		raison = " :Kicked by operator";
	else
		raison = " " + raison;
	for (int i = 0; i < salon.socketSize(); i++)
	{
		int socket = salon.get_SocketClient(i);
		client = salon.get_client(socket);
		if (client.getNickname() == username)
		{
			std::string kick_msg = ":" + clean(kicker.getNickname()) + "!" + clean(kicker.getUsername()) + "@127.0.0.1 KICK #" + clean(salon.getName()) + " " + clean(client.getNickname()) + raison + "\r\n";
			salon.send_to_all(kick_msg);
			salon.remove_client(client.getSocket());
			return;
		}
	}
	std::string error = ":127.0.0.1 441 " + kicker.getNickname() + " " + username + " #" + clean(salon.getName()) + " :They aren't on that channel\r\n";
	send(kicker.getSocket(), error.c_str(), error.size(), 0);
}

void Command::invite(std::string command, Salon &salon, std::map<int, Client> &clients, Client client)
{
	std::string username = command.substr(command.find(" ") + 1, command.find("#") - command.find(" ") - 2);
	std::map<int, Client> salon_clients = salon.get_all_client();
	bool is_in = false;
	for (int i = 0; i < salon.socketSize(); i++)
	{
		int socket = salon.get_SocketClient(i);
		if (salon_clients[socket].getNickname() == client.getNickname())
		{
			is_in = true;
			break;
		}
	}
	if (is_in == false)
	{
		std::string error = "'127.0.0.1 442 " + client.getNickname() + " #" + clean(salon.getName()) + " :You're not on that channel\r\n";
		send(client.getSocket(), error.c_str(), error.size(), 0);
		return;
	}
	for (int i = 0; i < salon.socketSize(); i++)
	{
		int socket = salon.get_SocketClient(i);
		if (clients[socket].getNickname() == username)
		{
			std::string error = "'127.0.0.1 443 " + client.getNickname() + " " + username + " #" + clean(salon.getName()) + " :is already on channel\r\n";
			send(client.getSocket(), error.c_str(), error.size(), 0);
			return;
		}
	}
	for (size_t i = 4; (i - 4) < clients.size(); i++)
	{
		if (clean(clients[i].getNickname()) == username)
		{
			std::string invite_msg = ":" + client.getNickname() + "!" + client.getUsername() + "@127.0.0.1 INVITE " + username + " :#" + clean(salon.getName()) + "\r\n";
			salon.set_invite(clients[i].getSocket());
			send(clients[i].getSocket(), invite_msg.c_str() , invite_msg.size(), 0);
			std::string error = ":127.0.0.1 341 " + client.getNickname() + " " + username + " #" + clean(salon.getName()) + "\r\n";
			send(client.getSocket(), error.c_str(), error.size(), 0);
			return;
		}
	}
	std::string error = ":127.0.0.1 401 " + client.getNickname() + " " + username + " :No such nick/channel\r\n";
	send(client.getSocket(), error.c_str(), error.size(), 0);
}

void Command::topic(std::string topics, Salon &salon, Client client)
{
	if (salon.get_mode(1) == true)
	{
		bool is_op = false;
		is_op = salon.is_operator(client.getSocket());
		if (is_op == false)
		{
			std::string error = ":127.0.0.1 482 " + clean(client.getNickname()) + " #" + clean(salon.getName()) + " :You're not channel operator\r\n";
			send(client.getSocket(), error.c_str(), error.size(), 0);
			return;
		}
	}
	std::string topic = clean(topics.substr(topics.find(":") + 1));
	if (topic.empty())
	{
		std::string topic_msg;
		if (salon.get_topic().empty())
			topic_msg = ":127.0.0.1 331 " + client.getNickname() + " #" + salon.getName() + " :No topic is set\r\n";
		else
			topic_msg = ":127.0.0.1 332 " + client.getNickname() + " #" + salon.getName() + " :" + salon.get_topic() + "\r\n";
		salon.send_to_all(topic_msg);
	}
	else
	{
		salon.set_topic(topic);
		std::string topic_msg = ":" + client.getNickname()+ "!" + client.getUsername() + "@" + "127.0.0.1" + " TOPIC #" + clean(salon.getName()) + " :" + topic + "\r\n";
		salon.send_to_all(topic_msg);
	}
}

void Command::mode(std::string args, Salon &salon, Client client)
{
	bool is_in = false;
	bool is_op = false;
	std::map<int, Client> clients = salon.get_all_client();
	for (int i = 0; i < salon.socketSize(); i++)
	{
		int socket = salon.get_SocketClient(i);
		if (clients[socket].getNickname() == client.getNickname())
		{
			is_in = true;
			break;
		}
	}
	if (is_in == false)
	{
		std::string error = "'127.0.0.1 442 " + client.getNickname() + " #" + clean(salon.getName()) + " :You're not on that channel\r\n";
		send(client.getSocket(), error.c_str(), error.size(), 0);
		return;
	}
	for (int i = 0; i < salon._operator_size(); i++)
	{
		if (clients[salon.get_SocketClient(i)].getNickname() == client.getNickname())
		{
			is_op = true;
			break;
		}
	}
	if (is_op == false)
	{
		std::string error = ":127.0.0.1 482 " + client.getNickname() + " #" + clean(salon.getName()) + " :You're not channel operator\r\n";
		send(client.getSocket(), error.c_str(), error.size(), 0);
		return;
	}
	size_t start = args.find("-");
	if (start == std::string::npos)
		start = args.find("+");
	if (start == std::string::npos)
	{
		std::string error = ":127.0.1 501 " + client.getNickname() + " #" + clean(salon.getName()) + " :Unknown mode\r\n";
		send(client.getSocket(), error.c_str(), error.size(), 0);
		return;
	}
	size_t end = args.find(" ", start);
	if (end == std::string::npos)
		end = args.find("\r", start);
	std::string option = args.substr(start, end - start);
	if (option.empty())
	{
		std::string error = ":127.0.1 501 " + client.getNickname() + " #" + clean(salon.getName()) + " :Unknown mode\r\n";
		send(client.getSocket(), error.c_str(), error.size(), 0);
		return;
	}
	start = args.find("\r", end);
	std::string value = args.substr(end + 1, start - end - 1);
	if (args.find("-") != std::string::npos)
		minus_mode(option, salon, client, value);
	else
		plus_mode(option, value, salon, client);
}

void Command::minus_mode(std::string option, Salon &salon, Client client, std::string value)
{
	if (option == "-i")
	{
		salon.set_mode(false, 0);
		std::string msg = ":" + client.getNickname() + "!" + client.getUsername() + "@127.0.0.1 MODE #" + clean(salon.getName()) + " -i\r\n";
		salon.send_to_all(msg);
	}
	else if (option == "-t")
	{
		salon.set_mode(false, 1);
		std::string msg = ":" + client.getNickname() + "!" + client.getUsername() + "@127.0.0.1 MODE #" + clean(salon.getName()) + " -t\r\n";
		salon.send_to_all(msg);
	}
	else if (option == "-k")
	{
		salon.set_mode(false, 2);
		salon.set_password("");
		std::string msg = ":" + client.getNickname() + "!" + client.getUsername() + "@127.0.1 MODE #" + clean(salon.getName()) + " -k\r\n";
		salon.send_to_all(msg);
	}
	else if (option == "-o")
	{
		Client user;
		for (int i = 0; i < salon.socketSize(); i++)
		{
			int socket = salon.get_SocketClient(i);
			user = salon.get_client(socket);
			if (user.getNickname() == value)
			{
				salon.remove_operator(user.getSocket());
				std::string msg = ":" + client.getNickname() + "!" + client.getUsername() + "@127.0.1 MODE #" + clean(salon.getName()) + " -o " + user.getNickname() + "\r\n";
				salon.send_to_all(msg);
				return;
			}
		}
		std::string error = ":127.0.1 441 " + client.getNickname() + " " + value + " #" + clean(salon.getName()) + " :They aren't on that channel\r\n";
		send(client.getSocket(), error.c_str(), error.size(), 0);
	}
	else if (option == "-l")
	{
		salon.set_mode(false, 3);
		salon.set_client_limits(0);
		std::string msg = ":" + client.getNickname() + "!" + client.getUsername() + "@127.0.1 MODE #" + clean(salon.getName()) + " -l\r\n";
		salon.send_to_all(msg);
	}
	else
	{
		std::string error = ":127.0.1 501 " + client.getNickname() + " #" + clean(salon.getName()) + " :Unknown mode\r\n";
		send(client.getSocket(), error.c_str(), error.size(), 0);
	}
}

void Command::plus_mode(std::string option, std::string value, Salon &salon, Client client)
{
	if (option == "+i")
	{
		salon.set_mode(true, 0);
		std::string msg = ":" + client.getNickname() + "!" + client.getUsername() + "@127.0.0.1 MODE #" + clean(salon.getName()) + " +i\r\n";
		salon.send_to_all(msg);
	}
	else if (option == "+t")
	{
		salon.set_mode(true, 1);
		std::string msg = ":" + client.getNickname() + "!" + client.getUsername() + "@127.0.0.1 MODE #" + clean(salon.getName()) + " +t\r\n";
		salon.send_to_all(msg);
	}
	else if (option == "+k")
	{
		salon.set_mode(true, 2);
		salon.set_password(value);
		std::string msg = ":" + client.getNickname() + "!" + client.getUsername() + "@127.0.1 MODE #" + clean(salon.getName()) + " +k\r\n";
		salon.send_to_all(msg);
	}
	else if (option == "+o")
	{
		Client user;
		for (int i = 0; i < salon.socketSize(); i++)
		{
			int socket = salon.get_SocketClient(i);
			user = salon.get_client(socket);
			if (user.getNickname() == value)
			{
				salon.set_operator(user.getSocket());
				std::string msg = ":" + client.getNickname() + "!" + client.getUsername() + "@127.0.1 MODE #" + clean(salon.getName()) + " +o " + user.getNickname() + "\r\n";
				salon.send_to_all(msg);
				return;
			}
		}
		std::string error = ":127.0.1 441 " + client.getNickname() + " " + value + " #" + clean(salon.getName()) + " :They aren't on that channel\r\n";
		send(client.getSocket(), error.c_str(), error.size(), 0);
	}
	else if (option == "+l")
	{
		salon.set_mode(true, 3);
		salon.set_client_limits(atoi(value.c_str()));
		std::string msg = ":" + client.getNickname() + "!" + client.getUsername() + "@127.0.1 MODE #" + clean(salon.getName()) + " +l\r\n";
		salon.send_to_all(msg);
	}
	else
	{
		std::string error = ":127.0.1 501 " + client.getNickname() + " #" + clean(salon.getName()) + " :Unknown mode\r\n";
		send(client.getSocket(), error.c_str(), error.size(), 0);
	}
}

