/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HelpCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 19:05:54 by tglory            #+#    #+#             */
/*   Updated: 2022/05/20 17:19:27 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/HelpCommand.hpp"
#include "../../includes/commands/CommandManager.hpp"

namespace ft {

	HelpCommand::HelpCommand() : ClientCommand("HELP") {}

	HelpCommand::~HelpCommand() {}

	bool HelpCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		ServerIRC *server = cmd.getServer();
		std::vector<ClientCommand*> commands = server->getCommandManager()->getCommands();
		std::string prefix = "006 " + client->getNick() + " ";
		std::stringstream ss;
		
		ss << prefix << "HELP - " << commands.size() << " commands" << client->getDelimiter();
		for (std::vector<ClientCommand*>::iterator it = commands.begin(); it != commands.end(); ++it) {
			ClientCommand *clientCmd = *it;
			ss << prefix << clientCmd->getName() << client->getDelimiter();
		}
		client->recieveMessage(ss.str());
		return true;
	}
}
