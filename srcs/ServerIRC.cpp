/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerIRC.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 18:10:32 by tglory            #+#    #+#             */
/*   Updated: 2022/04/20 21:51:08 by tglory           ###   ########lyon.fr   */
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
		return true;
	}

	bool ServerIRC::stop() {
		if (!this->enabled) {
			std::cerr << WARN << "Can't stop server IRC, he is not enabled." << C_RESET << std::endl;
			return false;
		}
		closesocket(sock);
		this->enabled = false;
		return true;
	}

	void ServerIRC::task() {
		SOCKADDR_IN csin;
    	SOCKET csock;
		while (1)
		{
			socklen_t sinsize = sizeof(csin);
			if ((csock = accept(sock, (SOCKADDR *)&csin, &sinsize)) != INVALID_SOCKET) {
				std::cout << C_BLUE << "A client logged in, we said 'Hello world'." << C_RESET << std::endl;
				send(csock, "Hello world!\r\n", 14, 0);
				break;
			}
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