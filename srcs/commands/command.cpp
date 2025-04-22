/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ertupop <ertupop@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 10:48:14 by rostrub           #+#    #+#             */
/*   Updated: 2025/04/22 08:46:37 by ertupop          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"

Command::Command()
{
}

Command::~Command()
{
}

void Command::selectCommand(std::string command, Salon &salon, Client client)
{	std::cout << "===================================================" << std::endl;
	if (command.find("KICK") != std::string::npos)
	{
		size_t start = command.find("KICK") + 6 + salon.getName().size();
		size_t end = command.find_first_of(" :", start);
		std::string username = command.substr(start, end - start);
		kick(username, salon);
	}
	else if (command.find("INVITE") != std::string::npos)
	{
		std::cout << "je suis la commande invite" << std::endl;
	}
	else if (command.find("TOPIC") != std::string::npos)
	{
		std::cout << "je suis la commande topic" << std::endl;
		topic(command, salon, client);
	}
	else if (command.find("MODE") != std::string::npos)
	{
		std::cout << "je suis la commande mode" << std::endl;
	}
	(void) salon;
}

void debug_print(const std::string& msg)
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

void Command::kick(std::string username, Salon &salon)
{
	Client client;
	for (int i = 0; i < salon.get_salon_client_len(); i++)
	{
		client = salon.get_client(i + 4);
		if (client.getNickname() == username)
		{
			std::string kick_msg = ":127.0.0.1 KICK #" + clean(salon.getName()) + " " + clean(client.getNickname()) + " :Kicked from channel\r\n";
			salon.send_to_all(kick_msg);
			debug_print(kick_msg);
			salon.remove_client(client.getSocket());
			return;
		}
	}
}



void Command::invite(std::string username, Salon &salon)
{
	(void) salon;
	std::cout << "Inviting " << username << std::endl;
}

void Command::topic(std::string topics, Salon &salon, Client client)
{
	std::string topic = clean(topics.substr(topics.find(":") + 1));
	std::cout << "==================" << std::endl;
	std::cout << "Topic = ";
	debug_print(topic);
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
		std::cout << "topic message = ";
		debug_print(topic_msg);
		for (size_t i = 0; i < topic_msg.size(); ++i)
    		std::cout << std::hex << (int)(unsigned char)topic_msg[i] << " ";
		std::cout << std::endl;
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


