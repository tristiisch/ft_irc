/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerIRC.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 18:10:32 by tglory            #+#    #+#             */
/*   Updated: 2022/05/07 21:14:18 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerIRC.hpp"
#include "../includes/ClientIRC.hpp"
#include <sys/errno.h>

namespace ft {

	ServerIRC::ServerIRC() : enabled(false), clientIdCounter(1) {
	/*	pfds[0].fd = STDIN_FILENO;
		pfds[0].events = POLLIN;
		//pfds[1].fd = serverSocket;
		pfds[1].events = POLLIN | POLLPRI | POLLHUP | POLLERR;*/


	}

	ServerIRC::~ServerIRC() {
		if (enabled)
			stop();
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

	void ServerIRC::task() {
		ClientIRC *client;
		SOCKADDR_IN csin;
    	SOCKET clientSocket;
		socklen_t sinsize = sizeof(csin);
		int ret, receiveByte;
		//char msg[] = "Hello world!\r\n";

		if (!isEnabled())
			exit(0);

		std::cout << C_BLUE << "Poll start." << C_RESET << std::endl;
		while ((ret = poll(&pfds[0], pfds.size(), 1 * 1000)) != -1) { 
			std::cout << C_YELLOW << "Poll ret " << ret << C_RESET << std::endl;
			//std::cout << C_BLUE << "Polllllllllll." << C_RESET << std::endl;
			/*if (pfds[0].revents & POLLIN) {
				char *receiveMsg;
				receiveMsg = (char*) std::calloc(512, 1);
				receiveByte = read(clientSocket, receiveMsg, sizeof(receiveMsg)); // This will block current thread
				if (ft::checkError(receiveByte, "Error while read connection", &csin)) {
					closesocket(clientSocket);
					return;
				}
				std::cout << C_BLUE << "STDIN receive: '" C_YELLOW << receiveMsg << C_BLUE << "'." << C_RESET << std::endl;
				free(receiveMsg);
				// read data from stdin and send it over the socket
				//read(STDIN_FILENO, ...);
				//std::cout << C_BLUE << "msg receive." << C_RESET << std::endl;
			
				// ret = send(clientSocket, msg, std::strlen(msg), 0) == -1;
				// if (!ft::checkError(ret, "Error while sending Hello world msg to ", &csin)) {
				// 	std::cout << C_BLUE << "A client logged in, we said 'Hello world'." << C_RESET << std::endl;
				// }
			}*/

			if (pfds[0].revents == POLLIN)
				acceptClient();
			else
				for (std::vector<pollfd>::iterator it = pfds.begin(); it != pfds.end(); ++it) {
					if ((*it).fd == 0)
					if ((*it).revents == POLLIN) {
						readClient(this->clients[(*it).fd]);
						//this->clients[(*it).fd]->receive(this);
					}
				}

			/*if (pfds[1].revents & POLLIN) {
				std::cout << C_BLUE << "POLLIN receive." << C_RESET << std::endl;

				// chat data received
				//recv(socket, ...)
				while (true) {
					char *receiveMsg;
					receiveMsg = (char*) std::calloc(512, 1);
					receiveByte = recv(clientSocket, receiveMsg, 512, 0); // This will block current thread
					if (receiveByte == 0)
						break;
					if (receiveByte == -1 && errno == EAGAIN)
						continue;
					if (ft::checkError(receiveByte, "Error while read connection", &csin)) {
						std::cout << C_RED << errno << C_RESET << std::endl;
						closesocket(clientSocket);
						return;
					}

					// VALGRIND: Conditional jump or move depends on uninitialised value(s)
					//std::cout << C_BLUE << "Message receive from " << csin << ": '" C_YELLOW << receiveMsg << C_BLUE << "'." << C_RESET << std::endl;
					this->executeCmds(client, receiveMsg);
					free(receiveMsg);
				}
			}*/
			/*if (pfds[1].revents & POLLPRI) {
				std::cout << C_BLUE << "POLLRI receive." << C_RESET << std::endl;
			}
			if (pfds[1].revents & POLLNVAL) {
				std::cout << C_BLUE << "Invalid request from " << C_RESET << std::endl;
				break;
			}
			if (pfds[1].revents & (POLLERR | POLLHUP)) {
				std::cout << C_RED << "sock close." << C_RESET << std::endl;
				// socket was closed
			}*/
		}

		std::cout << C_BLUE << "Poll end." << C_RESET << std::endl;


		/*pollfd pollfd;
		pollfd.fd = serverSocket;
		pollfd.events = POLLIN;
		ret = poll(&pollfd, nfds++, 60);
		ft::checkError(ret, "Error while using poll ", (char*) NULL);
		std::cout << "poll " << pollfd.revents << std::endl;*/

		/*while (true) {
			char *receiveMsg;
			receiveMsg = (char*) std::calloc(512, 1);
			receiveByte = recv(clientSocket, receiveMsg, 512, 0); // This will block current thread
			if (receiveByte == 0)
				break;
			if (ft::checkError(receiveByte, "Error while read connection", &csin)) {
				closesocket(clientSocket);
				return;
			}

			// VALGRIND: Conditional jump or move depends on uninitialised value(s)
			//std::cout << C_BLUE << "Message receive from " << csin << ": '" C_YELLOW << receiveMsg << C_BLUE << "'." << C_RESET << std::endl;
			client->executeCmd(receiveMsg);
			free(receiveMsg);
		}*/

		/*ret = send(clientSocket, msg, std::strlen(msg), 0) == -1;
		if (!ft::checkError(ret, "Error while sending Hello world msg to ", &csin)) {
			std::cout << C_BLUE << "'Hello world' >> " << csin << "." << C_RESET << std::endl;
		}*/
		// clients.insert(std::pair<int, ClientIRC*>(client->getId(), client));
		//closesocket(clientSocket);
	}

	void ServerIRC::acceptClient() {
		ClientIRC *client;
		SOCKADDR_IN csin;
    	SOCKET clientSocket;
		socklen_t sinsize = sizeof(csin);
		int ret;

		clientSocket = accept(serverSocket, (SOCKADDR *)&csin, &sinsize); // This will block current thread
		if (ft::checkError(clientSocket, "Error while accept connection", &csin)) {
			closesocket(clientSocket);
			return;
		}
		std::cout << C_BLUE << "A client " << csin << " logged in." << C_RESET << std::endl;
		client = new ClientIRC(this->getNewClientId(), csin, clientSocket);
		clients.insert(std::pair<int, ClientIRC*>(clientSocket, client));


		pfds.push_back(pollfd());
		pfds.back().events = POLLIN;
		pfds.back().fd = clientSocket;

		/*ret = fcntl(clientSocket, F_SETFL, O_NONBLOCK);
		if (ft::checkError(ret, "Error while use fcntl", (char*) NULL)) {
			return;
		}*/
	}

	bool ServerIRC::readClient(ClientIRC *client) {
		int receiveByte;

		char *receiveMsg;
		receiveMsg = (char*) std::calloc(512, 1);
		receiveByte = recv(serverSocket, receiveMsg, 512, 0); // This will block current thread
		if (receiveByte == 0) {
			free(receiveMsg);
			return false;
		}
		if (receiveByte == -1 && errno == EAGAIN) {
			free(receiveMsg);
			return true;
		}
		if (ft::checkError(receiveByte, "Error while read connection", &client->getSockAddr())) {
			std::cout << C_RED << errno << C_RESET << std::endl;
			//closesocket(clientSocket);
			free(receiveMsg);
			return true;
		}

		// VALGRIND: Conditional jump or move depends on uninitialised value(s)
		//std::cout << C_BLUE << "Message receive from " << csin << ": '" C_YELLOW << receiveMsg << C_BLUE << "'." << C_RESET << std::endl;
		this->executeCmds(client, receiveMsg);
		free(receiveMsg);
		return true;
	}

	bool ServerIRC::stop() {
		if (!this->enabled) {
			std::cerr << WARN << "Can't stop server IRC, he is not enabled." << C_RESET << std::endl;
			return false;
		}
        //std::for_each(clients.begin(), clients.end(), &ServerIRC::deleteClient);

		std::map<int, ClientIRC*>::iterator it;
		ClientIRC *client;
		for (it = clients.begin(); it != clients.end(); it++) {
			client = it->second;
			for (std::vector<pollfd>::iterator it_pfd = pfds.begin(); it_pfd != pfds.end(); ++it_pfd) {
				if ((*it_pfd).fd == client->getSocket()) {
					pfds.erase(it_pfd);
					break;
				}
			}
			delete client;
		}
		clients.clear();
		closesocket(serverSocket);
		this->enabled = false;
		std::cout << C_RED << "ft_irc stopped" << C_RESET << std::endl;
		return true;
	}

	void ServerIRC::executeCmds(ClientIRC *client, std::string bufferCmds) {
		std::string token1;
		std::string delim = "\n";
		size_t pos = 0;

		while ((pos = bufferCmds.find(delim)) != std::string::npos)
		{
			token1 = bufferCmds.substr(0, pos - 1);
			executeCmd(client, token1);
			bufferCmds.erase(0, pos + delim.length());
		}
		if (!bufferCmds.empty())
			executeCmd(client, token1);
	}

	void ServerIRC::executeCmd(ClientIRC *client, const std::string& fullCmd) {
		size_t index = fullCmd.find(" ");
		if (index == std::string::npos)
			return;

		std::string cmd = fullCmd.substr(0, index);
		std::string args = fullCmd.substr(index + 1, fullCmd.size());

		if (cmd == "NICK") {
			client->setNick(args);
			std::cout << C_YELLOW << "Nick of " << client->getSockAddr() << " is now '" C_YELLOW << args << C_BLUE << "'." << C_RESET << std::endl;
		} else if (cmd == "PASS") {
			if (!isGoodPassword(args)) {
				std::cout << C_RED << "Password '" << args << "' send by " << client->getSockAddr() << " is not server password ." << C_RESET << std::endl;
				return;
			}
			client->setAuthorized(true);
			std::cout << C_YELLOW << "Client " << client->getSockAddr() << " has good server password." << C_RESET << std::endl;
		} else {
			std::cout << C_BLUE << "Message receive from " << client->getSockAddr() << ": '" C_YELLOW << fullCmd << C_BLUE << "'." << C_RESET << std::endl;
		}
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
