/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostrub <rostrub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 10:48:14 by rostrub           #+#    #+#             */
/*   Updated: 2025/04/29 14:21:11 by rostrub          ###   ########.fr       */
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
	std::cout << "===================================================" << std::endl;
	std::cout << "Command: " << command << std::endl;
	std::cout << "Client: " << client.getNickname() << std::endl;
	std::cout << "===================================================" << std::endl;
	std::cout << "Find salon" << std::endl;

	std::cout << "============================================" << std::endl;
	if (command.find("KICK") != std::string::npos)
	{
		Salon salon;
		for (size_t i = 0; i < _salon.size(); ++i)
		{
			size_t start = command.find("#") + 1;
			size_t end = command.find(" ", start);
			if (clean(_salon[i].getName()) == command.substr(start, end - start))
			{
				salon = _salon[i];
				kick(command, salon, client);
				return;
			}
		}
		size_t start = command.find("#") + 1;
		size_t end = command.find(" ", start);
		std::string channel = command.substr(start, end - start);
		std::string error = "	:127.0.0.1 403 " + client.getNickname() + " #" + channel + " :No such channel\r\n";
		debug_print(error);
		send(client.getSocket(), error.c_str(), error.size(), 0);
		return;
	}
	else if (command.find("INVITE") != std::string::npos)
	{
		Salon salon;
		size_t start = command.find("#") + 1;
		size_t end = command.find("\r", start);
		for (size_t i = 0; i < _salon.size(); ++i)
		{
			if (clean(_salon[i].getName()) == command.substr(start, end - start))
			{
				salon = _salon[i];
				invite(command, _salon[i], clients, client);
				return;
			}
		}
		if (salon.getName().empty())
		{
			std::string error = "'127.0.0.1 442 " + client.getNickname() + " #" + command.substr(start, end - start) + " :You're not on that channel\r\n";
			send(client.getSocket(), error.c_str(), error.size(), 0);
			return;
		}
	}
	else if (command.find("TOPIC") != std::string::npos)
	{
		Salon salon;
		for (size_t i = 0; i < _salon.size(); ++i)
		{
			size_t start = command.find("#") + 1;
			size_t end = command.find(" ", start);
			if (clean(_salon[i].getName()) == command.substr(start, end - start))
			{
				salon = _salon[i];
				break;
			}
		}
		if (salon.getName().empty())
		{
			std::cout << "Salon not found" << std::endl;
			return;
		}
		topic(command, salon, client);
	}
	else if (command.find("MODE") != std::string::npos)
	{
		std::cout << "je suis la commande mode" << std::endl;
	}
}

void Command::kick(std::string command, Salon &salon, Client kicker)
{
	Client client;
	bool is_in = false;
	for (int i = 0; i < salon.get_salon_client_len(); i++)
	{
		client = salon.get_client(i + 4);
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
	size_t start = command.find("KICK") + 6 + salon.getName().size();
	size_t end = command.find_first_of(" :", start);
	std::string username = command.substr(start, end - start);
	start = command.find(":");
	end = command.find("\r", start);
	std::string raison = command.substr(start, end - start);
	if (raison.empty())
		raison = ":Kicked by operator";
	else
		raison = " " + raison;
	for (int i = 0; i < salon.get_salon_client_len(); i++)
	{
		client = salon.get_client(i + 4);
		if (client.getNickname() == username)
		{
			std::string kick_msg = ":" + kicker.getNickname() + "!" + " KICK #" + clean(salon.getName()) + " " + clean(client.getNickname()) + raison + "\r\n";
			salon.send_to_all(kick_msg);
			debug_print(kick_msg);
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
	bool is_in = false;
	for (int i = 0; i < salon.get_salon_client_len(); i++)
	{
		if (clients[i + 4].getNickname() == client.getNickname())
		{
			is_in = true;
			break;
		}
	}
	if (is_in == false)
	{
		std::string error = "'127.0.0.1 442 " + client.getNickname() + " #" + clean(salon.getName()) + " :You're not on that channel\r\n";
			debug_print(error);
			send(client.getSocket(), error.c_str(), error.size(), 0);
			return;
	}
	for (int i = 0; i < salon.get_salon_client_len(); i++)
	{
		if (clients[i + 4].getNickname() == username)
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

void Command::mode(std::string args, Salon &salon)
{
	if (args.empty())
	{
		salon.setMessage("You need to specifie arguments to execute this command");
		salon.showMessage();
	}
	if (args.find("-i"))
	{
		if (salon.get_opt(0) == true)
			salon.set_opt(0, false);
		else
			salon.set_opt(0, true);
	}
	else if (args.find("-t"))
	{
		if (salon.get_opt(1) == true)
			salon.set_opt(1, false);
		else
			salon.set_opt(1, true);
	}
	else if (args.find("-k"))
	{
		if (salon.get_opt(2) == true)
		{
			salon.set_opt(2, false);
			salon.set_password(NULL);
		}
		else
		{
			salon.set_opt(2, true);
			std::string password = args.substr(args.find("-k") + 3);
			if (password.empty())
			{
				salon.setMessage("Invalid arguments");
				salon.showMessage();
				return;
			}
			salon.set_password(args.substr(args.find("-k") + 3));
		}

	}
	else if (args.find("-l"))
	{
		if (salon.get_opt(3) == true)
			salon.set_opt(3, false);
		else
		{
			salon.set_opt(3, true);
			 int i = std::atoi(args.substr(args.find("-l") + 3).c_str());
			if (i < 0 || (i == 0 && args.substr(args.find("-l") + 3) != "0"))
			{
				salon.setMessage("Invalid arguments");
				salon.showMessage();
				return;
			}
			// salon.set_client_limits(std::stoi(args.substr(args.find("-l") + 3)));
		}
	}
	else
	{
		salon.setMessage("Invalid arguments");
		salon.showMessage();
	}

}


