/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 18:10:10 by tglory            #+#    #+#             */
/*   Updated: 2022/05/08 18:33:41 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.hpp"

namespace ft {
	class ServerConfig {

		private:
			int port;
			std::string ip;
			std::string password;

		public :
			ServerConfig();

			ServerConfig(int& port, std::string& password);

			ServerConfig& operator=(const ServerConfig& x);

			~ServerConfig() {}

			bool set(const int ac, const char *av[]);

			const int& getPort() const;
			bool setPort(const int& port);
			bool setPort(const char *port);

			const std::string& getPassword() const;
			bool setPassword(const std::string& password);
	
			bool setIP(const std::string& ip);
			const std::string& getIP() const;
	};
}
