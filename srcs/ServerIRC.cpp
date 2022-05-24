/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerIRC.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 18:10:32 by tglory            #+#    #+#             */
/*   Updated: 2022/05/23 20:58:39 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerIRC.hpp"
#include "../includes/ClientIRC.hpp"
#include "../includes/commands/ClientCommand.hpp"
#include "../includes/commands/QuitCommand.hpp"
#include "../includes/commands/CapCommand.hpp"
#include "../includes/commands/NickCommand.hpp"
#include "../includes/commands/PassCommand.hpp"
#include "../includes/commands/CommandManager.hpp"
#include "../includes/commands/CommandContext.hpp"

namespace ft {

	ServerIRC::ServerIRC() : enabled(false), clientIdCounter(1) {
		this->commandManager = new CommandManager(this);
	}

	ServerIRC::~ServerIRC() {
		if (enabled)
			stop();
		delete this->commandManager;
	}

	ServerIRC& ServerIRC::operator=(const ServerIRC& x) {
		this->setConfig(x.getConfig());
		return *this;
	}


	const std::vector<ChannelIRC*>& ServerIRC::getChannels() const
	{
		return this->channels;
	}

	const std::map<int, ClientIRC *>& ServerIRC::getClients() const
	{
		return this->clients;
	}


	bool ServerIRC::start() {
		SOCKADDR_IN sin;
		std::stringstream ss;
		int ret;

		sin.sin_addr.s_addr = inet_addr(config.getIP().c_str());
		sin.sin_family = PF_INET;
		sin.sin_port = htons(config.getPort());

		pfds.push_back(pollfd());
		pfds.back().events = POLLIN;
		pfds.back().fd = socket(PF_INET, SOCK_STREAM, 0);
		serverSocket = pfds.back().fd;
		if (ft::checkError(serverSocket, "Error with socket creation", (char*) NULL))
			return false;
		this->enabled = true;

		ret = fcntl(serverSocket, F_SETFL, O_NONBLOCK);
		if (ft::checkError(ret, "Error while use fcntl", (char*) NULL)) {
			stop();
			return false;
		}

		int enable = 1;
		ret = setsockopt(serverSocket, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(enable));
		if (ft::checkError(ret, "Error while use setsockopt SO_REUSEPORT", (char*) NULL)) {
			stop();
			return false;
		}

		enable = 1;
		ret = setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
		if (ft::checkError(ret, "Error while use setsockopt SO_REUSEADDR", (char*) NULL)) {
			stop();
			return false;
		}
	
		ret = bind(serverSocket, (SOCKADDR *)&sin, sizeof(sin));
		if (ft::checkError(ret, "Error while binding port", &config.getPort())) {
			stop();
			return false;
		}

		ret = listen(serverSocket, 42); // 42 = length max of queue
		if (ft::checkError(ret, "Error while listen port", &config.getPort())) {
			stop();
			return false;
		}

		ss << C_GREEN << "ft_irc started on port " << config.getPort() << C_RESET << std::endl;
		logAndPrint(ss.str());
		return true;
	}

	bool ServerIRC::stop() {
		if (!this->enabled) {
			std::stringstream ss;
			ss << WARN << "Can't stop server IRC, it is not enabled." << C_RESET << std::endl;
			logAndPrint(ss.str());
			return false;
		}
		this->enabled = false;

		if (!clients.empty()) {
			for (std::map<int, ClientIRC*>::iterator it = clients.begin(); it != clients.end();) {
				ClientIRC *client = it++->second;
				// client->closeSocket(); -> heap-use-after-free
				// delete client;
				// clients.erase(it);

				if (!client) {
					std::stringstream ss;
					ss << WARN << "map<fd, ClientIRC*> clients contains uninitialized instance of client." << C_RESET << std::endl;
					logAndPrint(ss.str());
					// clients.erase(it);
					continue;
				}
				deleteClient(client);
			}
			clients.clear();
		}
		closesocket(serverSocket);
		channels.clear();
		if (!pfds.empty()) {
			/*for (std::vector<pollfd>::iterator it = pfds.begin(); it != pfds.end(); ++it) {
				pfds.erase(it);
				// delete *it;
			}*/
			pfds.clear();
		}
		std::stringstream ss;
		ss << INFO << C_RED << "ft_irc stopped" << C_RESET << std::endl;
		logAndPrint(ss.str());
		return true;
	}

	void ServerIRC::execute() {
		std::stringstream ss;
		int ret;

		if (!isEnabled())
			exit(0);

		if (DEBUG_MODE) {
			ss << INFO << "Poll start with " << pfds.size() << " poll open" << C_RESET << std::endl;
			logAndPrint(ss.str());
			ss.clear();
		}
		while (this->enabled && !pfds.empty() && (ret = poll(&pfds[0], pfds.size(), 1 * 1000) != -1)) {
			if (pfds[0].revents & POLLIN)
				acceptClient(); // serverSocket receive connection
			else if (pfds.size() > 1) {
				// std::cout << C_GREEN << "Poll size " << pfds.size() << "." << C_RESET << std::endl;
				for (std::vector<pollfd>::iterator it = pfds.begin(); it != pfds.end(); ++it) { // clientsSockets receive connection
					if (it == pfds.begin()) {
						continue;
					}
					if (!this->isEnabled()) {
						std::cout << C_BLUE << "popopo STOP HERE" << C_RESET << std::endl;
						break;
					}
					// std::cout << C_GREEN << "POLL FOR " << pfds.size() << "." << C_RESET << std::endl;
					if (it->fd < 0) {
						ss << INFO << "Pollfd " << it->fd << " negative." << C_RESET << std::endl;
						logAndPrint(ss.str());
						ss.clear();
						break;
					}
					if (clients.find(it->fd) == clients.end()) {
						if (DEBUG_MODE) {
							ss << DEBUG << "Pollfd " << it->fd << " not linked to fd." << C_RESET << std::endl;
							logAndPrint(ss.str());
							ss.clear();
						}
						// deleteClient(this->clients[it->fd]);
						// pfds.erase(it);
						break;
					}
					if (it->revents & POLLIN) {
						// std::cout << C_BLUE << "Socket " << it->fd << " > POLLIN receive." << C_RESET << std::endl;
						readClient(this->clients[it->fd], it->fd);
					}
					if (it->revents & POLLPRI) {
						std::cout << C_BLUE << "Socket " << it->fd << " > POLLRI receive." << C_RESET << std::endl;
					}
					if (it->revents & POLLNVAL) {
						std::cout << C_BLUE << "Socket " << it->fd <<  " > Invalid request from" << C_RESET << std::endl;
					}
					if (it->revents & (POLLERR | POLLHUP)) {
						std::cout << C_RED << "Socket " << it->fd << " - " << *this->clients[it->fd] << " > close." << C_RESET << std::endl;
						deleteClient(this->clients[it->fd]);
						// break;
					}
				}
			}
			// if (DEBUG_MODE) {
			// 	std::stringstream ss;
			// 	ss << DEBUG << "next POOL | last ret : " << ret << " polls open : " << pfds.size() << " isEnabled : " << this->enabled << C_RESET << std::endl;
			// 	logAndPrint(ss.str());
			// }
		}
		if (ret == -1) {
			// if (errno == EINTR) {
			// 	exit(1);
			// }
			ft::checkError(ret, "Error while using POLL", &errno);
		}
		if (DEBUG_MODE) {
			ss << DEBUG << "Poll end" << C_RESET << std::endl;
			logAndPrint(ss.str());
		}
	}

	ClientIRC *ServerIRC::acceptClient() {
		ClientIRC *client;
		std::stringstream ss;
		SOCKADDR_IN csin;
    	SOCKET clientSocket;
		socklen_t sinsize = sizeof(csin);
		int ret;

		clientSocket = accept(serverSocket, (SOCKADDR *)&csin, &sinsize); // This will block current thread
		if (ft::checkError(clientSocket, "Error while accept connection", &csin)) {
			closesocket(clientSocket);
			return NULL;
		}
		client = new ClientIRC(this->getNewClientId(), csin, clientSocket);
		clients.insert(std::pair<int, ClientIRC*>(clientSocket, client));
		ss << INFO << "A client " << csin << " " << *client << " logged in." << C_RESET << std::endl;
		logAndPrint(ss.str());

		pfds.push_back(pollfd());
		// pfds.back().events = POLLIN | POLLPRI;
		pfds.back().events = POLLIN;
		pfds.back().fd = clientSocket;
		client->setPoll(pfds.back());
		client->recieveMessage("Hello world!");

		ret = fcntl(clientSocket, F_SETFL, O_NONBLOCK);
		if (ft::checkError(ret, "Error while use fcntl", (char*) NULL)) {
			return NULL;
		}
		return client;
	}

	bool ServerIRC::readClient(ClientIRC *client, SOCKET &socket) {
		int receiveByte;
		char *receiveMsg;

		if (!client) {
			std::stringstream ss;
			ss << WARN << "Unable to read null client with socket " << socket << C_RESET << std::endl;
			logAndPrint(ss.str());
			return false;
		}
		receiveMsg = (char*) std::calloc(512, 1);
		receiveByte = recv(socket, receiveMsg, 512, 0);
		if (receiveByte == 0) {
			free(receiveMsg);
			return false;
		}
		if (receiveByte == -1 && errno == EAGAIN) {
			free(receiveMsg);
			return false;
		}
		if (ft::checkError(receiveByte, "Error while read socket", client)) {
			free(receiveMsg);
			deleteClient(client);
			return false;
		}
		this->commandManager->receiveCmd(client, receiveMsg);
		free(receiveMsg);
		return true;
	}

	void ServerIRC::deleteClient(ClientIRC *client) {
		for (std::vector<ChannelIRC*>::const_iterator it = this->getChannels().begin(); it != this->getChannels().end(); ++it) {
			ChannelIRC *channel = *it;
			channel->clearUser(client);
		}
		if (!pfds.empty()) {
			for (std::vector<pollfd>::iterator it = pfds.begin() + 1; it != pfds.end(); ++it) {
				if (it->fd == client->getPoll().fd) {
					if (DEBUG_MODE) {
						std::stringstream ss;
						ss << DEBUG << "Delete poll of client : " << it->fd << C_RESET << std::endl;
						logAndPrint(ss.str());
					}
					pfds.erase(it);
					break;
				}
			}
		} else {
			std::stringstream ss;
			ss << WARN << "Unable to find clients poll." << C_RESET << std::endl;
			logAndPrint(ss.str());
		}
		std::map<int, ClientIRC*>::iterator it = clients.find(client->getSocket());
		if (it != clients.end()) {
			if (DEBUG_MODE) {
				std::stringstream ss;
				ss << DEBUG << "Delete client : " << *it->second << C_RESET << std::endl;
				logAndPrint(ss.str());
			}
			clients.erase(it);
		}
		client->closeSocket();
		delete client;
	}

	bool ServerIRC::isGoodPassword(std::string& password) {
		std::string serverPassword = this->getConfig().getPassword();
		if (serverPassword.empty() || serverPassword == password) 
			return true;
		return false;
	}

	int ServerIRC::getNewClientId() {
		return clientIdCounter++;
	}

	bool ServerIRC::isEnabled() const {
		return this->enabled;
	}

	const ServerConfig& ServerIRC::getConfig() const {
		return this->config;
	}

	bool ServerIRC::setConfig(const ServerConfig& config) {
		if (this->enabled)
			return false;
		this->config = config;
		return true;
	}

	ChannelIRC* ServerIRC::getChannel(std::string& channelName) const {
		std::vector<ChannelIRC*>::const_iterator channel = getChannels().begin();

		while (channel != getChannels().end()) {
			if ((*channel)->getName() == channelName)
				return *channel;
			channel++;
		}
		return NULL;
	}

	ClientIRC* ServerIRC::getClientByNick(std::string& clientNickname) const {
		for (std::map<int, ClientIRC*>::const_iterator it = getClients().begin(); it != getClients().end(); ++it) {
			if (it->second->getNick() != "" && it->second->getNick() == clientNickname) {
				return it->second;
			}
		}
		return NULL;
	}

	ClientIRC* ServerIRC::getClientBySocket(SOCKET& socket) const {
		for (std::map<int, ClientIRC*>::const_iterator it = getClients().begin(); it != getClients().end(); ++it) {
			if (it->second->getSocket() == socket) {
				return it->second;
			}
		}
		return NULL;
	}
	ClientIRC* ServerIRC::getClientById(int& id) const {
		for (std::map<int, ClientIRC*>::const_iterator it = getClients().begin(); it != getClients().end(); ++it) {
			if (it->second->getId() == id) {
				return it->second;
			}
		}
		return NULL;
	}

	void ServerIRC::addChannel(ChannelIRC* channel) {
		channels.push_back(channel);
	}

	CommandManager* ServerIRC::getCommandManager() {
		return this->commandManager;
	}
}
