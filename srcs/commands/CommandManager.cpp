/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allanganoun <allanganoun@student.42lyon    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 00:14:58 by tglory            #+#    #+#             */
/*   Updated: 2022/05/15 16:45:36 by allanganoun      ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/CommandManager.hpp"
#include "../../includes/commands/ClientCommand.hpp"
#include "../../includes/commands/QuitCommand.hpp"
#include "../../includes/commands/CapCommand.hpp"
#include "../../includes/commands/NickCommand.hpp"
#include "../../includes/commands/KickCommand.hpp"
#include "../../includes/commands/PrivateMsgCommand.hpp"
#include "../../includes/commands/NoticeCommand.hpp"
#include "../../includes/commands/PassCommand.hpp"
#include "../../includes/commands/UserCommand.hpp"
#include "../../includes/commands/CommandContext.hpp"

#include <vector>

namespace ft {

	// CommandManager::CommandManager() {}

	CommandManager::CommandManager(ServerIRC *server) : server(server) {
		commands.push_back(new QuitCommand());
		commands.push_back(new CapCommand());
		commands.push_back(new NickCommand());
		commands.push_back(new PassCommand());
		commands.push_back(new KickCommand());
		commands.push_back(new PrivateMsgCommand());
		commands.push_back(new UserCommand());
		commands.push_back(new NoticeCommand());

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

	void CommandManager::receiveCmd(ClientIRC *client, std::string bufferCmds) {
		std::string str;
		std::string delim = client->getDelimiter();
		size_t pos = 0;

		if (DEBUG_MODE) {
			std::stringstream ss;
			ss << DEBUG << *client << " send '" << bufferCmds << "'" << C_RESET << std::endl;
			logAndPrint(ss.str());
		}
		while ((pos = bufferCmds.find(delim)) != std::string::npos) {
			str = bufferCmds.substr(0, pos);
			executeCmd(client, str);
			bufferCmds.erase(0, pos + delim.length());
		}
		if (!bufferCmds.empty())
			executeCmd(client, bufferCmds);
	}

	bool CommandManager::executeCmd(ClientIRC *client, std::string& fullCmd) {
		std::string cmd, args;
		std::vector<std::string> argsArray;
		size_t index = fullCmd.find(" ");

		logCommand(client, fullCmd);
		if (index != std::string::npos) {
			cmd = fullCmd.substr(0, index);
			args = fullCmd.substr(index + 1, fullCmd.size());
			argsArray = split(args, " ");
		} else {
			cmd = fullCmd;
		}

		for (std::vector<ClientCommand*>::const_iterator it = commands.begin(); it != commands.end(); ++it) {
			ClientCommand *command = *it;

			if (command->getName() == cmd) {
				if (command->isNeededToBeAutorized() && !client->isAuthorized()) {
					std::stringstream ss;
					ss << INFO << C_RED << *client << " can't use command '" << C_BLUE << fullCmd << C_RED << "', he didn't enter the server password." << C_RESET << std::endl;
					logAndPrint(ss.str());
					return false;
				} else if (command->isNeededToBeOperator() && !client->isAuthorized()) {
					std::stringstream ss;
					ss << INFO << C_RED << *client << " can't use command '" << C_BLUE << fullCmd << C_RED << "', he is not operator." << C_RESET << std::endl;
					logAndPrint(ss.str());
					return false;
				}
				CommandContext commandContext(this->server, client, fullCmd, cmd, argsArray);
				return command->execute(commandContext);
			}
		}
		std::stringstream ss;
		ss << INFO << "Unknown message receive from " << *client << ": '" C_YELLOW << fullCmd << C_BLUE << "'." << C_RESET << std::endl;
		logAndPrint(ss.str());
		return false;
	}

	const ServerIRC* CommandManager::getServer() const {
		return this->server;
	}
}
