/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OperCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 19:05:54 by tglory            #+#    #+#             */
/*   Updated: 2022/05/25 19:38:59 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/OperCommand.hpp"

namespace ft {

	OperCommand::OperCommand() : ClientCommand("OPER", 2, "Add a server operator", "<nickname> <password>", true, false) {}

	OperCommand::~OperCommand() {}

	bool OperCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		ServerIRC *server = cmd.getServer();
		ClientIRC *target = server->getClientByNick(cmd.getArg(0));

		if (!target) {
			client->recieveMessage(ERR_NOSUCHNICK(cmd.getArg(0)));
			return false;
		}
	
		if (target->isOperator()) {
			return false;
		}

		if (!server->isGoodPassword(cmd.getArg(1))) {
			client->recieveMessage(ERR_PASSWDMISMATCH(client->getNick()));
			return false;
		}
		target->recieveMessage(RPL_YOUREOPER(client->getNick()));
		target->setOperator(true);
		return true;
	}
}
