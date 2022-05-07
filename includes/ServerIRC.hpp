/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerIRC.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 21:19:26 by tglory            #+#    #+#             */
/*   Updated: 2022/05/07 19:44:39 by tglory           ###   ########lyon.fr   */
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
			int clientIdCounter; // Last Client Id
			pollfd pfds[2];

		public :
			ServerIRC();

			//ServerIRC(ServerConfig& config) : enabled(false), config(config), nfds(1) {}

			ServerIRC& operator=(const ServerIRC& x);

			~ServerIRC();

			bool start();

			void task();

			bool stop();
			
			void executeCmds(ClientIRC *client, std::string bufferCmds);

			void executeCmd(ClientIRC *client, const std::string& fullCmd);

			bool isGoodPassword(std::string& password);

			const ServerConfig& getConfig() const;

			bool isEnabled() const;

			bool setConfig(const ServerConfig& config);

			int getNewClientId();
	};
}
