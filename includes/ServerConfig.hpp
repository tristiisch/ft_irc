/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 18:10:10 by tglory            #+#    #+#             */
/*   Updated: 2022/04/19 18:28:43 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.hpp"

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

		const int& getPort() const;

		bool setPort(const int& port);

		const std::string& getPassword() const;

		bool setPassword(const std::string& password);
};
