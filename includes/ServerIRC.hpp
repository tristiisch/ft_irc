/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerIRC.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 21:19:26 by tglory            #+#    #+#             */
/*   Updated: 2022/05/06 21:02:16 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include "ft_irc.hpp"
#include "ServerConfig.hpp"
#include "ClientIRC.hpp"

namespace ft {
	class ServerIRC {

		private:
			bool enabled;
			ServerConfig config;
			SOCKET serverSocket;
			std::map<int, ClientIRC*> clients;
			int nfds; // Last Client Id
			pollfd pfds[2];

		public :
			ServerIRC() : enabled(false), nfds(1) {
				pfds[0].fd = STDIN_FILENO;
				pfds[0].events = POLLIN;
				pfds[1].fd = serverSocket;
				pfds[1].events = POLLIN;
			}

			//ServerIRC(ServerConfig& config) : enabled(false), config(config), nfds(1) {}

			ServerIRC& operator=(const ServerIRC& x) {
				this->setConfig(x.getConfig());
				return *this;
			}

			~ServerIRC() {
				if (enabled)
					stop();
			}

			const ServerConfig& getConfig() const;

			bool isEnabled() const;

			bool setConfig(const ServerConfig& config);

			bool start();

			void task();

			bool stop();

			int getNewClientId();
	};
}
