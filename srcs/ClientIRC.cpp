/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientIRC.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 16:35:51 by tglory            #+#    #+#             */
/*   Updated: 2022/05/12 17:29:36 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ClientIRC.hpp"

namespace ft {

	ClientIRC::ClientIRC(int id, SOCKADDR_IN& sockAddr, SOCKET& clientSocket) : id(id), sockAddr(sockAddr), clientSocket(clientSocket), nick(""), authorized(false) {}

	ClientIRC& ClientIRC::operator=(const ClientIRC& x) {
		this->id = x.getId();
		this->sockAddr = x.getSockAddr();
		this->clientSocket = x.getSocket();
		return *this;
	}

	ClientIRC::~ClientIRC() {}

	bool ClientIRC::closeSocket() {
		if (this->clientSocket <= 0)
			return false;
		closesocket(this->clientSocket);
		return true;
	}

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

	const bool& ClientIRC::isAuthorized() const {
		return this->authorized;
	}

	pollfd& ClientIRC::getPoll() {
		return this->poll;
	}

	void ClientIRC::setSocket(SOCKET& clientSocket) {
		this->clientSocket = clientSocket;
	}

	void ClientIRC::setNick(std::string& nick) {
		this->nick = nick;
	}

	void ClientIRC::setAuthorized(const bool& authorized) {
		this->authorized = authorized;
	}

	void ClientIRC::setPoll(pollfd& poll) {
		this->poll = poll;
	}

	void ClientIRC::sendMessage(ClientIRC *const &to, std::string const &message)\
	{

	}
}
