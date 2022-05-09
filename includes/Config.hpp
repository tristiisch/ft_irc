/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allanganoun <allanganoun@student.42lyon    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 15:01:20 by allanganoun       #+#    #+#             */
/*   Updated: 2022/05/05 15:02:45 by allanganoun      ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Utils.hpp"

namespace ft {
	class ServerConfig {

		private:
			int port;
			std::string password;

		public :
			ServerConfig() : port(-1), password("") {}

			ServerConfig(int& port, std::string& password) : port(port), password(password) {}

			ServerConfig& operator=(const ServerConfig& x)
			{
				this->setPassword(x.getPassword());
				this->setPort(x.getPort());
				return *this;
			}

			~ServerConfig() {}

			bool set(const int ac, const char *av[]);

			const int& getPort() const;

			bool setPort(const int& port);

			bool setPort(const char *port);

			const std::string& getPassword() const;

			bool setPassword(const std::string& password);
	};
}
