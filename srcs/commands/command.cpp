/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostrub <rostrub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 10:48:14 by rostrub           #+#    #+#             */
/*   Updated: 2025/03/05 10:48:49 by rostrub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"

Command::Command()
{
}

Command::~Command()
{
}

void Command::kick(std::string username, Salon *salon)
{
	std::cout << "Kicking " << username << std::endl;
}

void Command::invite(std::string username, Salon *salon)
{
	std::cout << "Inviting " << username << std::endl;
}

void Command::topic(std::string topics, Salon *salon)
{
	if (topics.empty())
	{
		salon->setMessage(salon->get_topic());
		salon->showMessage();
	}
	else
	{
		salon->set_topic(topics);
	}
}

void Command::mode(std::string args, Salon *salon)
{
	if (args.empty())
	{
		salon->setMessage("You need to specifie arguments to execute this command");
		salon->showMessage();
	}
	if (args.find("-i"))
	{
		if (salon->get_opt(0) == true)
			salon->set_opt(0, false);
		else
			salon->set_opt(0, true);
	}
	else if (args.find("-t"))
	{
		if (salon->get_opt(1) == true)
			salon->set_opt(1, false);
		else
			salon->set_opt(1, true);
	}
	else if (args.find("-k"))
	{
		if (salon->get_opt(2) == true)
		{
			salon->set_opt(2, false);
			salon->set_password(NULL);
		}
		else
		{
			salon->set_opt(2, true);
			std::string password = args.substr(args.find("-k") + 3);
			if (password.empty())
			{
				salon->setMessage("Invalid arguments");
				salon->showMessage();
				return;
			}
			salon->set_password(args.substr(args.find("-k") + 3));
		}

	}
	else if (args.find("-l"))
	{
		if (salon->get_opt(3) == true)
			salon->set_opt(3, false);
		else
		{
			salon->set_opt(3, true);
			int i = std::stoi(args.substr(args.find("-l") + 3));
			if (i < 0 || (i == 0 && args.substr(args.find("-l") + 3) != "0"))
			{
				salon->setMessage("Invalid arguments");
				salon->showMessage();
				return;
			}
			salon->set_client_limits(std::stoi(args.substr(args.find("-l") + 3)));
		}
	}
	else
	{
		salon->setMessage("Invalid arguments");
		salon->showMessage();
	}

}


