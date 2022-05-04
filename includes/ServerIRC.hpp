/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerIRC.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 21:19:26 by tglory            #+#    #+#             */
/*   Updated: 2022/05/04 18:00:44 by tglory           ###   ########lyon.fr   */
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

		public :
			ServerIRC() : enabled(false), nfds(1) {}

			ServerIRC(ServerConfig& config) : enabled(false), config(config), nfds(1) {}

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
