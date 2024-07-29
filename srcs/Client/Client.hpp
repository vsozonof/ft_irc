/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 10:15:47 by vsozonof          #+#    #+#             */
/*   Updated: 2024/07/29 10:59:11 by vsozonof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "../../includes/inc.hpp"

class Client
{
	private:
		int		_socket;			// Socket du client
		bool	_authenticated;		// Si le client est authentifié ou non

	public:
		int				getSocket() const;
		int 			isAuth() const;
		void			setAuth(bool auth);
		std::string		receiveMsg();
		void 			sendMsg(std::string const &msg);
		void 			closeClient();

		Client(int socket);
		~Client();
};

#endif