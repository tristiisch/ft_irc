/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 18:10:32 by tglory            #+#    #+#             */
/*   Updated: 2022/04/20 01:46:23 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerConfig.hpp"

namespace ft {
	const int& ServerConfig::getPort() const {
		return this->port;
	}

	bool ServerConfig::setPort(const char* port) {
		int intPort = std::atoi(port);
		if (strspn(port, "0123456789") != strlen(port))
			return false;
		return setPort(intPort);
	}

	bool ServerConfig::setPort(const int& port) {
		if (port < 0 || port > 65535)
			return false;
		this->port = port;
		return true;
	}

	const std::string& ServerConfig::getPassword() const {
		return this->password;
	}

	bool ServerConfig::setPassword(const std::string& password) {
		this->password = password;
		return true;
	}
}