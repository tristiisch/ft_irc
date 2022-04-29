/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerIRC.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 18:10:32 by tglory            #+#    #+#             */
/*   Updated: 2022/04/29 17:53:34 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerIRC.hpp"

namespace ft {
	
	bool ServerIRC::start() {
		SOCKADDR_IN sin;
		int ret;

		sin.sin_addr.s_addr = inet_addr("0.0.0.0");
		sin.sin_family = PF_INET;
		sin.sin_port = htons(config.getPort());

		serverSock = socket(PF_INET, SOCK_STREAM, 0);
		if (ft::checkError(serverSock, "Error with socket creation", (char*) NULL))
			return false;
		this->enabled = true;
		ret = bind(serverSock, (SOCKADDR *)&sin, sizeof(sin));
		if (ft::checkError(ret, "Error while binding port", &config.getPort())) {
			stop();
			return false;
		}
		ret = listen(serverSock, 42); // 42 because bircd is set to 42
		if (ft::checkError(ret, "Error while listen port", &config.getPort())) {
			stop();
			return false;
		}
		std::cout << C_GREEN << "ft_irc started on port " << config.getPort() << C_RESET << std::endl;
		return true;
	}

	bool ServerIRC::stop() {
		if (!this->enabled) {
			std::cerr << WARN << "Can't stop server IRC, he is not enabled." << C_RESET << std::endl;
			return false;
		}
        std::for_each(clientSock.begin(), clientSock.end(), &close);
		closesocket(serverSock);
		this->enabled = false;
		std::cout << C_RED << "ft_irc stopped" << C_RESET << std::endl;
		return true;
	}

	void ServerIRC::task() {
		pollfd pfds[2];
		SOCKADDR_IN csin;
    	SOCKET csock;
		socklen_t sinsize = sizeof(csin);
		int ret;
		char msg[] = "Hello world!\r\n";
	
		pfds[0].fd = STDIN_FILENO;
		pfds[0].events = POLLIN;
		pfds[1].fd = serverSock;
		pfds[1].events = POLLIN;

		/*std::cout << C_BLUE << "Poll start." << C_RESET << std::endl;
		while (poll(pfds, nfds++, 60) != -1) {
			if (pfds[0].revents & POLLIN) {
				// read data from stdin and send it over the socket
				//read(STDIN_FILENO, ...);
				
				ret = send(serverSock, msg, std::strlen(msg), 0) == -1;
				if (!ft::checkError(ret, "Error while sending Hello world msg to ", &csin)) {
					std::cout << C_BLUE << "A client logged in, we said 'Hello world'." << C_RESET << std::endl;
				}
			}
			if (pfds[1].revents & POLLIN) {
				std::cout << C_BLUE << "msg receive." << C_RESET << std::endl;
				// chat data received
				//recv(socket, ...)
			}
			if (pfds[1].revents & (POLLERR | POLLHUP)) {
				// socket was closed
			}
		}*/

		//std::cout << C_BLUE << "Poll end." << C_RESET << std::endl;
		pollfd pollfd;
		pollfd.fd = serverSock;
		//pollfd.events = POLLIN;
		/*ret = poll(&pollfd, nfds++, 60);
		ft::checkError(ret, "Error while using poll ", (char*) NULL);
		std::cout << "poll " << pollfd.revents << std::endl;*/

		csock = accept(serverSock, (SOCKADDR *)&csin, &sinsize); // This will block current thread
		if (csock != INVALID_SOCKET) {
			
			ret = send(csock, msg, std::strlen(msg), 0) == -1;
			if (!ft::checkError(ret, "Error while sending Hello world msg to ", &csin)) {
				std::cout << C_BLUE << "A client " << csin << " logged in, we said 'Hello world'." << C_RESET << std::endl;
			}
			//closesocket(csock);
			clientSock.push_back(csock); // vector to close FDs at end of server csock (when ServerIRC::stop)
		}
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
