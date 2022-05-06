/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientIRC.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 16:35:51 by tglory            #+#    #+#             */
/*   Updated: 2022/05/06 20:09:59 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ClientIRC.hpp"

namespace ft {

	const int& ClientIRC::getId() const {
		return this->id;
	}

	const in_port_t& ClientIRC::getPort() const {
		return this->sockAddr.sin_port;
	}

	const in_addr& ClientIRC::getAddress() const {
		return this->sockAddr.sin_addr;
	}

	const SOCKADDR_IN& ClientIRC::getSockAddr() const {
		return this->sockAddr;
	}

	const SOCKET& ClientIRC::getSocket() const {
		return this->clientSocket;
	}

	const std::string& ClientIRC::getNick() const {
		return this->nick;
	}

	void ClientIRC::setSocket(SOCKET& clientSocket) {
		this->clientSocket = clientSocket;
	}

	void ClientIRC::setNick(std::string& nick) {
		this->nick = nick;
	}

	void ClientIRC::executeCmds(std::string bufferCmds) {
		std::string token1;
		std::string delim = "\n";
		size_t pos = 0;
		
		while ((pos = bufferCmds.find(delim)) != std::string::npos)
		{
			token1 = bufferCmds.substr(0, pos - 1);
			executeCmd(token1);
			bufferCmds.erase(0, pos + delim.length());
		}
		if (!bufferCmds.empty())
			executeCmd(token1);
	}

	void ClientIRC::executeCmd(std::string fullCmd) {
		size_t index = fullCmd.find(" ");
		if (index == std::string::npos)
			return;

		std::string cmd = fullCmd.substr(0, index);
		std::string args = fullCmd.substr(index + 1, fullCmd.size());
		std::cout << C_BLUE << "Message receive from " << getSockAddr() << ": '" C_YELLOW << cmd << " " << args << C_BLUE << "'." << C_RESET << std::endl;

		if (cmd == "NICK") {
			this->setNick(args);
			std::cout << C_BLUE << "Nick of " << getSockAddr() << " is now '" C_YELLOW << args << C_BLUE << "'." << C_RESET << std::endl;
		} //else if ()
	}
}
