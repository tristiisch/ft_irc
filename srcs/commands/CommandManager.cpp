/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 00:14:58 by tglory            #+#    #+#             */
/*   Updated: 2022/05/28 17:02:01 by tglory           ###   ########lyon.fr   */
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
#include "../../includes/commands/JoinCommand.hpp"
#include "../../includes/commands/UserCommand.hpp"
#include "../../includes/commands/CommandContext.hpp"
#include "../../includes/commands/PartCommand.hpp"
#include "../../includes/commands/ExitCommand.hpp"
#include "../../includes/commands/HelpCommand.hpp"
#include "../../includes/commands/OperCommand.hpp"
#include "../../includes/commands/ModeCommand.hpp"

namespace ft {

	CommandManager::CommandManager() {}

	CommandManager::CommandManager(ServerIRC *server) : server(server) {
		commands.push_back(new HelpCommand());
		commands.push_back(new QuitCommand());
		commands.push_back(new CapCommand());
		commands.push_back(new NickCommand());
		commands.push_back(new PassCommand());
		commands.push_back(new KickCommand());
		commands.push_back(new JoinCommand());
		commands.push_back(new PrivateMsgCommand());
		commands.push_back(new UserCommand());
		commands.push_back(new NoticeCommand());
		commands.push_back(new PartCommand());
		commands.push_back(new ExitCommand());
		commands.push_back(new OperCommand());
		commands.push_back(new ModeCommand());
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
		commands.clear();
	}

	void CommandManager::receiveCmd(ClientIRC *client, std::string bufferCmds) {
		std::stringstream ss;
		std::string str;
		std::string delim = client->getDelimiter();
		size_t pos = 0;

		if (DEBUG_MODE) {
			ss << C_BLUE << "<- " << *client << " send '" << bufferCmds << "'" << C_RESET << std::endl;
			logAndPrint(ss.str());
			ss.str("");
		}
		while ((pos = bufferCmds.find(delim)) != std::string::npos) {
			str = bufferCmds.substr(0, pos);
			executeCmd(client, str);
			bufferCmds.erase(0, pos + delim.length());
		}
		if (!bufferCmds.empty()) {
			ss << C_RED << "<- " << *client << " send '" << bufferCmds << "' without delimiter" << C_RESET << std::endl;
			logAndPrint(ss.str());
			// executeCmd(client, bufferCmds);
		}
	}

	bool CommandManager::executeCmd(ClientIRC *client, std::string& fullCmd) {
		std::string cmd, args;
		std::vector<std::string> argsArray;
		size_t index = fullCmd.find(" ");
		std::stringstream ss;

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
					ss << INFO << C_RED << *client << " can't use command '" << C_BLUE << fullCmd << C_RESET << C_RED << "', he didn't enter the server password." << C_RESET << std::endl;
					logAndPrint(ss.str());
					return false;
				} else if (command->isNeededToBeRegistered() && !client->isRegistered()) {
					ss << INFO << C_RED << *client << " can't use command '" << C_BLUE << fullCmd << C_RESET << C_RED << "', he is not registered." << C_RESET << std::endl;
					logAndPrint(ss.str());
					return false;
				} else if (command->isNeededToBeOperator() && !client->isOperator()) {
					ss << INFO << C_RED << *client << " can't use command '" << C_BLUE << fullCmd << C_RESET << C_RED << "', he is not operator." << C_RESET << std::endl;
					logAndPrint(ss.str());
					client->recieveMessage(ERR_NOPRIVILEGES);
					return false;
				}
				CommandContext commandContext(this->server, client, fullCmd, cmd, argsArray);

				if ((size_t) command->getMinArg() > commandContext.getArgs().size()) {
					client->recieveMessage(ERR_NEEDMOREPARAMS(command->getName()));
					return false;
				}
				return command->execute(commandContext);
			}
		}
		ss << INFO << "Unknown message receive from " << *client << ": '" C_YELLOW << fullCmd << C_GREEN << "'." << C_RESET << std::endl;
		logAndPrint(ss.str());
		return false;
	}

	const ServerIRC* CommandManager::getServer() const {
		return this->server;
	}

	std::vector<ClientCommand*>& CommandManager::getCommands() {
		return this->commands;
	}
}
