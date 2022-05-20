/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientIRC.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 16:35:51 by tglory            #+#    #+#             */
/*   Updated: 2022/05/20 17:46:42 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ClientIRC.hpp"

namespace ft {

	ClientIRC::ClientIRC(int id, SOCKADDR_IN& sockAddr, SOCKET& clientSocket) :
		id(id),
		sockAddr(sockAddr),
		clientSocket(clientSocket),
		nick(""),
		delimiter("\r\n"),
		authorized(false),
		registered(false)
	{}

	ClientIRC& ClientIRC::operator=(const ClientIRC& x) {
		this->id = x.getId();
		this->sockAddr = x.getSockAddr();
		this->clientSocket = x.getSocket();
		return *this;
	}

	ClientIRC::~ClientIRC() {}

	bool ClientIRC::operator ==(ClientIRC* const client) const {
		return (id == client->id);
	}

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

	const std::string& ClientIRC::getDelimiter() const {
		return this->delimiter;
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

	void ClientIRC::setDelimitator(const std::string& Delimiter) {
		this->delimiter = Delimiter;
	}

	void ClientIRC::sendMessage(ClientIRC *const &to, std::string const &message)\
	{
		std::string finished_msg = ":" + this->nick + " " + message;
		to->recieveMessage(finished_msg);
	}

	void	ClientIRC::recieveMessage(std::string const &message)
	{
		std::string msg_for_client = message + getDelimiter();

		if (DEBUG_MODE) {
			std::stringstream ss;
			ss << C_CYAN << "-> Server send to " << *this << " : '" << message << "'" << C_RESET << std::endl;
			logAndPrint(ss.str());
		}
		send(this->clientSocket, msg_for_client.c_str(), msg_for_client.length(), 0);
	}

	const bool& ClientIRC::isRegistered() const {
		return this->registered;
	}

	void ClientIRC::setRegistered(const bool& registered) {
		this->registered = registered;
	}

	const std::string& ClientIRC::getUsername() const {
		return this->username;
	}

	void ClientIRC::setUsername(std::string& username) {
		this->username = username;
	}
}
