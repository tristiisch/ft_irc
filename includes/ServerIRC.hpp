/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerIRC.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 21:19:26 by tglory            #+#    #+#             */
/*   Updated: 2022/05/25 19:12:40 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.hpp"
#include "ServerConfig.hpp"
#include "ClientIRC.hpp"
#include "ChannelIRC.hpp"


namespace ft {

	class CommandManager;
	
	class ServerIRC {

		private:
			bool enabled;
			bool tryingToStop;
			ServerConfig config;
			SOCKET serverSocket;
			std::map<int, ClientIRC*> clients;
			std::vector<ChannelIRC*> channels;
			int clientIdCounter;
			std::vector<pollfd> pollfds;
			CommandManager *commandManager;

		public :
			ServerIRC();
			ServerIRC& operator=(const ServerIRC& x);
			~ServerIRC();

			const std::vector<ChannelIRC*>& getChannels() const;
			const std::map<int, ClientIRC*>& getClients() const;
			
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
			bool isTryingToStop() const;
			void setTryingToStop();
			int getNewClientId();
			ClientIRC* getClientByNick(std::string& clientNickname) const;
			ClientIRC* getClientBySocket(SOCKET& socket) const;
			ClientIRC* getClientById(int& id) const;
			ChannelIRC* getChannel(std::string& channelName) const;
			void addChannel(ChannelIRC* channel);
			CommandManager* getCommandManager();
	};
}
