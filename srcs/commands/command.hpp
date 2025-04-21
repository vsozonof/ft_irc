/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostrub <rostrub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 10:43:57 by rostrub           #+#    #+#             */
/*   Updated: 2025/04/18 17:40:20 by rostrub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "../../includes/inc.hpp"
#include "../Salon/Salon.hpp"

class Salon;

class Command
{
	private:
		Command();
	public:
		~Command();
		static void selectCommand(std::string command, Salon &salon);
		static void kick(std::string username, Salon &salon);
		static void invite(std::string username, Salon &salon);
		static void topic(std::string topics, Salon &salon);
		static void mode(std::string args, Salon &salon);
};

#endif
