/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientIRC.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 16:32:12 by tglory            #+#    #+#             */
/*   Updated: 2022/05/09 16:21:52 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.hpp"

namespace ft {
	class ClientIRC {

		private:
			int id;
			SOCKADDR_IN sockAddr;
			SOCKET clientSocket;
			std::string nick;
			bool authorized;
			pollfd poll;

			ClientIRC() : id(-1) {}

		public :

			ClientIRC(int id, SOCKADDR_IN& sockAddr, SOCKET& clientSocket);
			ClientIRC& operator=(const ClientIRC& x);
			~ClientIRC();

			bool closeSocket();
			const int& getId() const;
			const in_port_t& getPort() const;
			const in_addr& getAddress() const;
			const SOCKADDR_IN& getSockAddr() const;
			const SOCKET& getSocket() const;
			const std::string& getNick() const;
			const bool& isAuthorized() const;
			pollfd& getPoll();
			void setSocket(SOCKET& clientSocket);
			void setNick(std::string& nick);
			void setAuthorized(const bool& authorized);
			void setPoll(pollfd& poll);

	};

	std::ostream &operator<<(std::ostream &outputFile, const ClientIRC &client);
}
