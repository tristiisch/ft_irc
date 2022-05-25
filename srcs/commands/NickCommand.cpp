/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 21:31:41 by tglory            #+#    #+#             */
/*   Updated: 2022/05/25 19:35:16 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/NickCommand.hpp"

namespace ft {

	NickCommand::NickCommand() : ClientCommand("NICK", 1, "Change your nickname", "<nickname>") {}

	NickCommand::~NickCommand() {}

	bool NickCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		ServerIRC *server = cmd.getServer();
		std::string newNick = cmd.getArg(0);
		std::stringstream ss;

		if (newNick.empty()) {
			ss << INFO << C_YELLOW << *client << " try to set a empty Nickname." << C_RESET << std::endl;
			logAndPrint(ss.str());
			return false;
		}
		if (server->getClientByNick(newNick)) {
			ss << INFO << C_YELLOW << *client << " try to set a nickname already used." << C_RESET << std::endl;
			logAndPrint(ss.str());
			client->recieveMessage(ERR_NICKNAMEINUSE(newNick));
			return false;
		}
		client->recieveMessage(RPL_NICK(newNick, client->getNick(), client->getUsername(), client->getHost()));
		ss << INFO << C_YELLOW << "Nick of " << *client << " is now '" << newNick << "'." << C_RESET << std::endl;
		logAndPrint(ss.str());
		client->setNick(newNick);
		return true;
	}

}
