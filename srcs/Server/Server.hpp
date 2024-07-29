/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:03:09 by vsozonof          #+#    #+#             */
/*   Updated: 2024/07/29 07:38:22 by vsozonof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "../../includes/inc.hpp"

class Server
{
	private:
		unsigned int				_port;
		std::string					_password;
		int							_socket;
		std::vector<struct pollfd>	_clients;

	public:
		Server(unsigned int port, std::string password);
		~Server();
		
		void initServer();
		void run();
		
};

#endif