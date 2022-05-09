/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerIRC.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 18:10:32 by tglory            #+#    #+#             */
/*   Updated: 2022/05/09 04:14:05 by tglory           ###   ########lyon.fr   */
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
	/*	pfds[0].fd = STDIN_FILENO;
		pfds[0].events = POLLIN;
		//pfds[1].fd = serverSocket;
		pfds[1].events = POLLIN | POLLPRI | POLLHUP | POLLERR;*/
		this->commandManager = new CommandManager(*this);
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

	bool ServerIRC::start() {
		SOCKADDR_IN sin;
		int ret;

		sin.sin_addr.s_addr = inet_addr("0.0.0.0");
		sin.sin_family = PF_INET;
		sin.sin_port = htons(config.getPort());

		pfds.push_back(pollfd());
		pfds.back().events = POLLIN;
		pfds.back().fd = socket(PF_INET, SOCK_STREAM, 0);
		serverSocket = pfds.back().fd;
		if (ft::checkError(serverSocket, "Error with socket creation", (char*) NULL))
			return false;
		this->enabled = true;

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

		/*ret = fcntl(serverSocket, F_SETFL, O_NONBLOCK);
		if (ft::checkError(ret, "Error while use fcntl", (char*) NULL)) {
			stop();
			return false;
		}*/
		std::cout << C_GREEN << "ft_irc started on port " << config.getPort() << C_RESET << std::endl;
		return true;
	}

	bool ServerIRC::stop() {
		ClientIRC *client;
	
		if (!this->enabled) {
			std::cerr << WARN << "Can't stop server IRC, he is not enabled." << C_RESET << std::endl;
			return false;
		}
		this->enabled = false;

		for (std::map<int, ClientIRC*>::iterator it = clients.begin(); it != clients.end(); ++it) {
			client = it->second;
			closesocket(client->getSocket());
			delete client;
			clients.erase(it);
		}
		closesocket(serverSocket);
		// clients.clear();
		for (std::vector<pollfd>::iterator it = pfds.begin(); it != pfds.end(); ++it) {
			// pfds.erase(it);
			// delete *it;
		}
		pfds.clear();
		std::cout << C_RED << "ft_irc stopped" << C_RESET << std::endl;
		return true;
	}

	void ServerIRC::execute() {
		char msg[] = "Hello world!\r\n";
		int ret;

		if (!isEnabled())
			exit(0);

		std::cout << C_BLUE << "Poll start with " << pfds.size() << "." << C_RESET << std::endl;
		while (this->enabled && ((ret = poll(&pfds[0], pfds.size(), 1 * 1000)) != -1)) {
			if (pfds[0].revents == POLLIN)
				acceptClient();
			else {
				for (std::vector<pollfd>::iterator it = pfds.begin() + 1; it != pfds.end(); ++it) {
					pollfd poll = *it;
					if (poll.revents & POLLIN) {
						readClient(this->clients[poll.fd]);
						ret = send(poll.fd, msg, std::strlen(msg), 0) == -1;
						ft::checkError(ret, "Error while sending Hello world msg to ", &this->clients[poll.fd]);
						//this->clients[(*it).fd]->receive(this);
					}
					if (poll.revents & POLLPRI) {
						std::cout << C_BLUE << "Socket " << poll.fd << " > POLLRI receive." << C_RESET << std::endl;
					}
					if (poll.revents & POLLNVAL) {
						std::cout << C_BLUE << "Socket " << poll.fd <<  " > Invalid request from" << C_RESET << std::endl;
						break;
					}
					if (poll.revents & (POLLERR | POLLHUP)) {
						// socket was closed
						std::cout << C_RED << "Socket " << poll.fd <<  " > close." << C_RESET << std::endl;
						// deleteClient(this->clients[(*it).fd]);
					}
				}
			}
			if (DEBUG) {
				std::cout << C_BLUE << "next POOL ret : " << ret << " size : " << pfds.size() << " isEnable : " << this->enabled << C_RESET << std::endl;
			}
		}
		if (ret == -1) {
			// if (errno == EINTR) {
			// 	exit(1);
			// }
			ft::checkError(ret, "Error while using POLL", &errno);
		}
		std::cout << C_BLUE << "Poll end." << C_RESET << std::endl;
	}

	ClientIRC *ServerIRC::acceptClient() {
		ClientIRC *client;
		SOCKADDR_IN csin;
    	SOCKET clientSocket;
		socklen_t sinsize = sizeof(csin);

		clientSocket = accept(serverSocket, (SOCKADDR *)&csin, &sinsize); // This will block current thread
		if (ft::checkError(clientSocket, "Error while accept connection", &csin)) {
			closesocket(clientSocket);
			return NULL;
		}
		std::cout << C_BLUE << "A client " << csin << " fd:" << clientSocket << " logged in." << C_RESET << std::endl;
		client = new ClientIRC(this->getNewClientId(), csin, clientSocket);
		clients.insert(std::pair<int, ClientIRC*>(clientSocket, client));

		pfds.push_back(pollfd());
		pfds.back().events = POLLIN;
		pfds.back().fd = clientSocket;
		client->setPoll(pfds.back());

		/*ret = fcntl(clientSocket, F_SETFL, O_NONBLOCK);
		if (ft::checkError(ret, "Error while use fcntl", (char*) NULL)) {
			return;
		}*/
		return client;
	}

	bool ServerIRC::readClient(ClientIRC *client) {

		int receiveByte;
		char *receiveMsg;

		if (client != NULL || client->getSocket() < 0)
			return false;
		receiveMsg = (char*) std::calloc(512, 1);
		receiveByte = recv(client->getSocket(), receiveMsg, 512, 0);
		if (receiveByte == 0) {
			free(receiveMsg);
			return false;
		}
		if (receiveByte == -1 && errno == EAGAIN) {
			free(receiveMsg);
			return false;
		}
		if (ft::checkError(receiveByte, "Error while read socket", client)) {
			// std::cout << C_RED << errno << C_RESET << std::endl;
			// closesocket(clientSocket);
			free(receiveMsg);
			return false;
		}
		this->commandManager->executeCmds(client, receiveMsg);
		free(receiveMsg);
		return true;
	}

	void ServerIRC::deleteClient(ClientIRC *client) {
		if (!pfds.empty()) {
			for (std::vector<pollfd>::iterator it = pfds.begin() + 1; it != pfds.end(); ++it) {
				std::cout << "test " << it->fd << " vs " << client->getPoll().fd << std::endl;
				if (it->fd == client->getPoll().fd) {
					pfds.erase(it);
					// delete client->getPoll();
					std::cout << "YESS" << std::endl;
					break;
				}
			}
		}
		closesocket(client->getSocket());
		clients.erase(client->getSocket());
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
}
