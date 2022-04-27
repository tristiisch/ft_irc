/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerIRC.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 18:10:32 by tglory            #+#    #+#             */
/*   Updated: 2022/04/27 18:22:15 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerIRC.hpp"

namespace ft {

	bool ServerIRC::start() {
		SOCKADDR_IN sin;
		int ret;

		sin.sin_addr.s_addr = inet_addr("127.0.0.1");
		sin.sin_family = PF_INET;
		sin.sin_port = htons(config.getPort());

		sock = socket(PF_INET, SOCK_STREAM, 0);
		if (ft::checkError(sock, "Error with socket creation", (char*) NULL))
			return false;
		this->enabled = true;
		ret = bind(sock, (SOCKADDR *)&sin, sizeof(sin));
		if (ft::checkError(ret, "Error while binding port", &config.getPort())) {
			stop();
			return false;
		}
		ret = listen(sock, 0);
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
		closesocket(sock);
		this->enabled = false;
		std::cout << C_RED << "ft_irc stopped" << C_RESET << std::endl;
		return true;
	}

	void ServerIRC::task() {
		SOCKADDR_IN csin;
    	SOCKET csock;
		socklen_t sinsize = sizeof(csin);
		char msg[] = "Hello world!\r\n";
	
		csock = accept(sock, (SOCKADDR *)&csin, &sinsize); // This will block current thread
		if (csock != INVALID_SOCKET) {
			std::cout << C_BLUE << "A client logged in, we said 'Hello world'." << C_RESET << std::endl;
			send(csock, msg, std::strlen(msg), 0);
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
