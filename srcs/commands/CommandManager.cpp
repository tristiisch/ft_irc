/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 00:14:58 by tglory            #+#    #+#             */
/*   Updated: 2022/05/12 05:19:12 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/CommandManager.hpp"
#include "../../includes/commands/ClientCommand.hpp"
#include "../../includes/commands/QuitCommand.hpp"
#include "../../includes/commands/CapCommand.hpp"
#include "../../includes/commands/NickCommand.hpp"
#include "../../includes/commands/PassCommand.hpp"
#include "../../includes/commands/CommandContext.hpp"
#include <vector>

namespace ft {
	
	// CommandManager::CommandManager() {}

	CommandManager::CommandManager(ServerIRC *server) : server(server) {
		commands.push_back(new QuitCommand());
		commands.push_back(new CapCommand());
		commands.push_back(new NickCommand());
		commands.push_back(new PassCommand());
	}
			
	CommandManager &CommandManager::operator=(CommandManager const &instance) {
		this->server = instance.server;
		this->commands = instance.commands;
		return *this;
	}

	CommandManager::~CommandManager() {
		for (std::vector<ClientCommand*>::iterator it = commands.begin(); it != commands.end(); ++it) {
			delete *it;
		}
	}

	void CommandManager::executeCmds(ClientIRC *client, std::string bufferCmds) {
		std::string token1;
		std::string delim = "\n";
		size_t pos = 0;

		while ((pos = bufferCmds.find(delim)) != std::string::npos) {
			token1 = bufferCmds.substr(0, pos);
			executeCmd(client, token1);
			bufferCmds.erase(0, pos + delim.length());
		}
		if (!bufferCmds.empty())
			executeCmd(client, token1);
	}

	void CommandManager::executeCmd(ClientIRC *client, std::string& fullCmd) {
		size_t index = fullCmd.find(" ");
		if (index == std::string::npos)
			return;

		std::string cmd = fullCmd.substr(0, index);
		std::string args = fullCmd.substr(index + 1, fullCmd.size());
		std::vector<std::string> argsArray = split(args, " ");

		for (std::vector<ClientCommand*>::const_iterator it = commands.begin(); it != commands.end(); ++it) {
			ClientCommand *command = *it;
			
			if (command->getName() == cmd) {
				if (command->isNeededToBeAutorized() && !client->isAuthorized()) {
					std::cout << C_RED << *client << " can't use command '" << C_BLUE << fullCmd << C_RED << "', he didn't enter the server password." << C_RESET << std::endl;
					return;
				} else if (command->isNeededToBeOperator() && !client->isAuthorized()) {
					std::cout << C_RED << *client << " can't use command '" << C_BLUE << fullCmd << C_RED << "', he is not operator." << C_RESET << std::endl;
					return;
				}
				CommandContext commandContext(this->server, client, fullCmd, cmd, argsArray);
				command->execute(commandContext);
				return;
			}
		}
		std::cout << C_BLUE << "Unknown message receive from " << *client << ": '" C_YELLOW << fullCmd << C_BLUE << "'." << C_RESET << std::endl;
	}

	const ServerIRC* CommandManager::getServer() const {
		return this->server;
	}
}