/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ertupop <ertupop@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 10:43:57 by rostrub           #+#    #+#             */
/*   Updated: 2025/04/16 12:31:06 by ertupop          ###   ########.fr       */
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
		void selectCommand(std::string command, Salon *salon);
		void kick(std::string username, Salon *salon);
		void invite(std::string username, Salon *salon);
		void topic(std::string topics, Salon *salon);
		void mode(std::string args, Salon *salon);
};

#endif
