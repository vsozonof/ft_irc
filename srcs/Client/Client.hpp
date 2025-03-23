/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotilli <tpotilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 10:15:47 by vsozonof          #+#    #+#             */
/*   Updated: 2025/03/21 13:55:51 by tpotilli         ###   ########.fr       */
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
		std::string _nickname;		// Pseudo du client
		std::string _username;		// Username du client
		std::string _password;		// Mot de passe du client



	public:

		int				getSocket() const;
		int 			isAuth() const;
		std::string		getNickname() const;
		std::string		getUsername() const;
		
		void			setAuth(bool auth);
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