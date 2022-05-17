/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerIRC.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 21:19:26 by tglory            #+#    #+#             */
/*   Updated: 2022/05/17 21:32:28 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.hpp"
#include "ServerConfig.hpp"
#include "ClientIRC.hpp"
#include "ChannelIRC.hpp"
// #include "commands/CommandManager.hpp"


namespace ft {

	class CommandManager;
	// class ServerIRC;
	class ServerIRC {

		private:
			bool enabled;
			ServerConfig config;
			SOCKET serverSocket;
			std::map<int, ClientIRC*> clients;
			std::vector<ChannelIRC> channels;
			int clientIdCounter; // Last Client Id
			std::vector<pollfd> pfds;
			CommandManager *commandManager;

		public :
			ServerIRC();
			ServerIRC& operator=(const ServerIRC& x);
			~ServerIRC();

			std::vector<ChannelIRC>&	getChannels();
			std::map<int, ClientIRC*>&	getClients();
			
			bool start();
			bool stop();
			void execute();
			ClientIRC* acceptClient();
			bool readClient(ClientIRC *client, SOCKET& socket);
			void deleteClient(ClientIRC *client);
			bool isGoodPassword(std::string& password);

			const ServerConfig& getConfig() const;
			bool isEnabled() const;
			bool setConfig(const ServerConfig& config);
			int getNewClientId();
	};
}
