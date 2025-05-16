/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 10:15:47 by vsozonof          #+#    #+#             */
/*   Updated: 2025/05/15 16:31:06 by vsozonof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "../../includes/inc.hpp"

class Client
{
	private:
		int		_socket;			// Socket du client
		std::string _nickname;		// Pseudo du client
		std::string _username;		// Username du client
		std::string _password;		// Mot de passe du client



	public:

		int				getSocket() const;
		std::string		getNickname() const;
		std::string		getUsername() const;
		
		void			setNickname(std::string const &nickname);
		void			setUsername(std::string const &username);
		
		std::string		receiveMsg();
		void 			sendMsg(std::string const &msg);
		void 			closeClient();

		Client();
		Client(int socket);
		Client(const Client &src);
		Client& operator=(Client const &rhs);
		~Client();
};

#endif