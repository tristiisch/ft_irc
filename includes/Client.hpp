/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allanganoun <allanganoun@student.42lyon    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 15:00:59 by allanganoun       #+#    #+#             */
/*   Updated: 2022/05/05 15:02:38 by allanganoun      ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Utils.hpp"

namespace ft {
	class ClientIRC {

		private:
			int id;
			SOCKADDR_IN sockAddr;
			SOCKET clientSocket;
			std::string nick;

		public :
			ClientIRC() : id(-1) {}

			ClientIRC(int id, SOCKADDR_IN& sockAddr, SOCKET& clientSocket) : id(id), sockAddr(sockAddr), clientSocket(clientSocket), nick("") {}

			ClientIRC& operator=(const ClientIRC& x)
			{
				this->id = x.getId();
				this->sockAddr = x.getSockAddr();
				this->clientSocket = x.getSocket();
				return *this;
			}

			~ClientIRC()
			{
				closesocket(clientSocket);
			}

			const int& getId() const;

			const in_port_t& getPort() const;

			const in_addr& getAddress() const;

			const SOCKADDR_IN& getSockAddr() const;

			const SOCKET& getSocket() const;

			const std::string& getNick() const;

			void setSocket(SOCKET& clientSocket);

			void setNick(std::string& nick);
	};
}
