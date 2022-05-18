/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 21:31:41 by tglory            #+#    #+#             */
/*   Updated: 2022/05/18 20:32:43 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/NickCommand.hpp"

namespace ft {

	NickCommand::NickCommand() : ClientCommand("NICK") {}

	NickCommand::~NickCommand() {}

	bool NickCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		ServerIRC *server = cmd.getServer();
		std::vector<std::string> args = cmd.getArgs();
		std::string newNick;

		if (args.empty()) {
			std::cout << C_YELLOW << *client << " try to set a empty Nickname." << C_RESET << std::endl;
			return false;
		}
		newNick = cmd.getArg(0);
		if (server->getClientByNick(newNick)) {
			std::cout << C_YELLOW << *client << " try to set a nickname already used." << C_RESET << std::endl;
			client->recieveMessage(ERR_NICKNAMEINUSE(newNick));
			return false;
		}
		// client->recieveMessage("NICK " + newNick);
		client->recieveMessage(RPL_NICK(newNick, client->getNick(), client->getUsername(), "localhost"));
		std::cout << C_YELLOW << "Nick of " << *client << " is now '" << newNick << "'." << C_RESET << std::endl;
		client->setNick(newNick);
		return true;
	}

}
