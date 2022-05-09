/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 00:11:55 by tglory            #+#    #+#             */
/*   Updated: 2022/05/09 02:30:25 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../ServerIRC.hpp"
#include "../ClientIRC.hpp"
#include "ClientCommand.hpp"

namespace ft {

	class CommandManager {

		private:
			ServerIRC server;
			std::vector<ClientCommand*> commands;

			CommandManager();
		public:
			CommandManager(ServerIRC& server);
			CommandManager &operator=(CommandManager const &instance);
			~CommandManager();
			void executeCmds(ClientIRC *client, std::string bufferCmds);
			void executeCmd(ClientIRC *client, std::string& fullCmd);
			const ServerIRC& getServer() const;
	};
}