/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientIRC.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 16:32:12 by tglory            #+#    #+#             */
/*   Updated: 2022/05/18 15:42:28 by tglory           ###   ########lyon.fr   */
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
			pollfd poll;
			std::string nick;
			std::string username;
			std::string delimiter;
			bool authorized;
			bool registered;

			ClientIRC() : id(-1) {}

		public :

			ClientIRC(int id, SOCKADDR_IN& sockAddr, SOCKET& clientSocket);
			ClientIRC& operator=(const ClientIRC& x);
			~ClientIRC();

			bool operator ==(ClientIRC* const client) const;

			bool closeSocket();
			const int& getId() const;
			const in_port_t& getPort() const;
			const in_addr& getAddress() const;
			const SOCKADDR_IN& getSockAddr() const;
			const SOCKET& getSocket() const;
			const std::string& getNick() const;
			const bool& isAuthorized() const;
			const std::string& getDelimiter() const;
			pollfd& getPoll();
			const bool& isRegistered() const;
			const std::string& getUsername() const;

			void setSocket(SOCKET& clientSocket);
			void setNick(std::string& nick);
			void setAuthorized(const bool& authorized);
			void setDelimitator(const std::string& Delimiter);
			void setPoll(pollfd& poll);
			void setRegistered(const bool& registered);
			void setUsername(std::string& username);

			void sendMessage(ClientIRC *const &to, std::string const &message);
			void recieveMessage(std::string const &message);
	};

	std::ostream &operator<<(std::ostream &outputFile, const ClientIRC &client);
}
