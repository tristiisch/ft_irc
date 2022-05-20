/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 00:11:55 by tglory            #+#    #+#             */
/*   Updated: 2022/05/20 14:53:22 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../ServerIRC.hpp"
#include "../ClientIRC.hpp"
#include "ClientCommand.hpp"

namespace ft {

	class CommandManager {

		private:
			ServerIRC *server;
			std::vector<ClientCommand*> commands;

			CommandManager();
		public:
			CommandManager(ServerIRC *server);
			CommandManager &operator=(CommandManager const &instance);
			~CommandManager();
			void receiveCmd(ClientIRC *client, std::string bufferCmds);
			bool executeCmd(ClientIRC *client, std::string& fullCmd);
			const ServerIRC* getServer() const;
			std::vector<ClientCommand*>& getCommands();
	};
}
