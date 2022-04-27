/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerIRC.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 18:10:32 by tglory            #+#    #+#             */
/*   Updated: 2022/04/28 01:19:43 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerIRC.hpp"

namespace ft {

	std::ostream &operator<<(std::ostream &outputFile, SOCKADDR_IN &csin)
	{
		outputFile << inet_ntoa(csin.sin_addr) << ":" << csin.sin_port;
		return outputFile;
	}
	
	bool ServerIRC::start() {
		SOCKADDR_IN sin;
		int ret;

		sin.sin_addr.s_addr = inet_addr("127.0.0.1");
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
		SOCKADDR_IN csin;
    	SOCKET csock;
		socklen_t sinsize = sizeof(csin);
		int ret;
		char msg[] = "Hello world!\r\n";
	
		csock = accept(serverSock, (SOCKADDR *)&csin, &sinsize); // This will block current thread
		if (csock != INVALID_SOCKET) {
			
			ret = send(csock, msg, std::strlen(msg), 0) == -1;
			if (!ft::checkError(ret, "Error while sending Hello world msg to ", &csin)) {
				std::cout << C_BLUE << "A client " << csin << " logged in, we said 'Hello world'." << C_RESET << std::endl;
			}
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
