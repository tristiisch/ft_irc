/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HelpCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 19:05:54 by tglory            #+#    #+#             */
/*   Updated: 2022/05/28 15:07:36 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/HelpCommand.hpp"

namespace ft {

	HelpCommand::HelpCommand() : ClientCommand("HELP", 0, "This message", "", false, false, false) {}

	HelpCommand::~HelpCommand() {}

	bool HelpCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		ServerIRC *server = cmd.getServer();
		std::vector<ClientCommand*> commands = server->getCommandManager()->getCommands();
		std::string prefix = "006 " + client->getNick() + " ";
		std::stringstream ss;
		
		ss << prefix << commands.size() << " commands" << client->getDelimiter();
		for (std::vector<ClientCommand*>::iterator it = commands.begin(); it != commands.end(); ++it) {
			ClientCommand *clientCmd = *it;
			ss << prefix << clientCmd->getName() << " " << clientCmd->getDescription();
			if (!clientCmd->getFormat().empty())
				ss << " -> " << clientCmd->getName() << " " << clientCmd->getFormat();
			ss << client->getDelimiter();
		}
		client->recieveMessage(ss.str());
		return true;
	}
}
