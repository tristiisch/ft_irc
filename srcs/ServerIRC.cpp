/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerIRC.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 18:10:32 by tglory            #+#    #+#             */
/*   Updated: 2022/05/28 13:26:20 by tglory           ###   ########lyon.fr   */
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

	ServerIRC::ServerIRC() : enabled(false), tryingToStop(false), clientIdCounter(1) {
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

		pollfds.push_back(pollfd());
		pollfds.back().events = POLLIN;
		pollfds.back().fd = socket(PF_INET, SOCK_STREAM, 0);
		serverSocket = pollfds.back().fd;
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

		/*ret = setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
		if (ft::checkError(ret, "Error while use setsockopt SO_REUSEADDR", (char*) NULL)) {
			stop();
			return false;
		}*/

		ret = bind(serverSocket, (SOCKADDR *)&sin, sizeof(sin));
		if (ft::checkError(ret, "Error while binding port", &config.getPort())) {
			stop();
			return false;
		}

		ret = listen(serverSocket, 512);
		if (ft::checkError(ret, "Error while listen port", &config.getPort())) {
			stop();
			return false;
		}

		ss << C_GREEN << "ft_irc started on port " << config.getPort() << C_RESET << std::endl;
		logAndPrint(ss.str());
		return true;
	}

	bool ServerIRC::stop() {
		std::stringstream ss;

		if (!this->enabled) {
			ss << WARN << "Can't stop server IRC, it is not enabled." << C_RESET << std::endl;
			logAndPrint(ss.str());
			ss.str("");
			return false;
		}
		this->enabled = false;

		if (!clients.empty()) {
			for (std::map<int, ClientIRC*>::iterator it = clients.begin(); it != clients.end();) {
				ClientIRC *client = it++->second;
				/*if (!client) {
					std::stringstream ss;
					ss << WARN << "map<fd, ClientIRC*> clients contains uninitialized instance of client." << C_RESET << std::endl;
					logAndPrint(ss.str());
					// clients.erase(it);
					continue;
				}*/
				deleteClient(client);
			}
			clients.clear();
		}
		closesocket(serverSocket);
		channels.clear();
		pollfds.clear();
		ss << INFO << C_RED << "ft_irc stopped" << C_RESET << std::endl;
		logAndPrint(ss.str());
		ss.str("");
		return true;
	}

	void ServerIRC::execute() {
		std::stringstream ss;
		int ret;

		if (!isEnabled())
			exit(0);

		// if (DEBUG_MODE) {
		// 	ss << INFO << "Poll start with " << pollfds.size() << " poll open" << C_RESET << std::endl;
		// 	logAndPrint(ss.str());
		// 	ss.str("");
		// }
		while (!this->tryingToStop && !pollfds.empty() && (ret = poll(&pollfds[0], pollfds.size(), 1 * 1000) != -1)) {
			if (pollfds[0].revents & POLLIN)
				acceptClient();
			else if (pollfds.size() > 1) {
				for (std::vector<pollfd>::iterator it = pollfds.begin(); !this->tryingToStop && it != pollfds.end(); ++it) {
					if (it == pollfds.begin())
						continue;

					if (it->fd < 0) {
						ss << INFO << "Pollfd " << it->fd << " negative." << C_RESET << std::endl;
						logAndPrint(ss.str());
						ss.str("");
						break;
					}
					if (clients.find(it->fd) == clients.end()) {
						if (DEBUG_MODE) {
							ss << DEBUG << "Pollfd " << it->fd << " not linked to fd." << C_RESET << std::endl;
							logAndPrint(ss.str());
							ss.str("");
						}
						break;
					}
					if (it->revents & (POLLERR | POLLHUP)) {
						ss << INFO << C_RED << "Socket " << it->fd << " - " << *this->clients[it->fd] << " > close." << C_RESET << std::endl;
						logAndPrint(ss.str());
						ss.str("");
						deleteClient(this->clients[it->fd]);
						break;
					} else if (it->revents & POLLIN) {
						// std::cout << C_BLUE << "Socket " << it->fd << " > POLLIN receive." << C_RESET << std::endl;
						readClient(this->clients[it->fd], it->fd);
						break;
					} else if (it->revents & POLLPRI) {
						std::cout << C_BLUE << "Socket " << it->fd << " > POLLRI receive." << C_RESET << std::endl;
					} else if (it->revents & POLLNVAL) {
						std::cout << C_BLUE << "Socket " << it->fd <<  " > Invalid request from" << C_RESET << std::endl;
					}
				}
			}
			// if (DEBUG_MODE) {
			// 	std::stringstream ss;
			// 	ss << DEBUG << "next POOL | last ret : " << ret << " polls open : " << pollfds.size() << " isEnabled : " << this->enabled << C_RESET << std::endl;
			// 	logAndPrint(ss.str());
			// }
		}
		if (ret == -1) {
			ft::checkError(ret, "Error while using POLL", &errno);
		}
		// if (DEBUG_MODE) {
		// 	ss << DEBUG << "Poll end" << C_RESET << std::endl;
		// 	logAndPrint(ss.str());
		// }
	}

	ClientIRC *ServerIRC::acceptClient() {
		ClientIRC *client;
		std::stringstream ss;
		SOCKADDR_IN csin;
    	SOCKET clientSocket;
		socklen_t sinsize = sizeof(csin);
		int ret;

		clientSocket = accept(serverSocket, (SOCKADDR *)&csin, &sinsize);
		if (ft::checkError(clientSocket, "Error while accept connection", &csin)) {
			closesocket(clientSocket);
			return NULL;
		}
		client = new ClientIRC(this->getNewClientId(), csin, clientSocket);
		clients.insert(std::pair<int, ClientIRC*>(clientSocket, client));
		ss << INFO << "A client " << *client << " logged in." << C_RESET << std::endl;
		logAndPrint(ss.str());

		pollfds.push_back(pollfd());
		pollfds.back().events = POLLIN | POLLPRI;
		pollfds.back().fd = clientSocket;
		client->setPoll(pollfds.back());
		//client->recieveMessage("Hello world!");

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
		if (!receiveByte) {
			free(receiveMsg);
			return false;
		}
		if (receiveByte == -1 && errno == EAGAIN) {
			std::cout << WARN << "read fail EAGAIN " << socket << C_RESET << std::endl;
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
			channel->sendMessageToAll(client, "PART " + channel->getName());
			channel->clearUser(client);
		}
		if (!pollfds.empty()) {
			for (std::vector<pollfd>::iterator it = pollfds.begin() + 1; it != pollfds.end(); ++it) {
				if (it->fd == client->getPoll().fd) {
					// if (DEBUG_MODE) {
					// 	std::stringstream ss;
					// 	ss << DEBUG << "Delete pollfd : " << *client<< C_RESET << std::endl;
					// 	logAndPrint(ss.str());
					// }
					pollfds.erase(it);
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


	bool ServerIRC::isTryingToStop() const{
		return this->tryingToStop;
	}
	
	void ServerIRC::setTryingToStop(){
		this->tryingToStop = true;
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
