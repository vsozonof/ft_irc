/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostrub <rostrub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 10:43:57 by rostrub           #+#    #+#             */
/*   Updated: 2025/04/21 19:14:23 by rostrub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "../../includes/inc.hpp"
#include "../Salon/Salon.hpp"
#include "../Client/Client.hpp"

class Salon;
class Client;

class Command
{
	private:
		Command();
	public:
		~Command();
		static std::string clean(std::string s);
		static void selectCommand(std::string command, Salon &salon, Client client);
		static void kick(std::string username, Salon &salon);
		static void invite(std::string username, Salon &salon);
		static void topic(std::string topics, Salon &salon, Client client);
		static void mode(std::string args, Salon &salon);
};

#endif
