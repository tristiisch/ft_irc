/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OperCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 19:05:54 by tglory            #+#    #+#             */
/*   Updated: 2022/05/23 17:08:49 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/OperCommand.hpp"

namespace ft {

	OperCommand::OperCommand() : ClientCommand("OPER") {}

	OperCommand::~OperCommand() {}

	bool OperCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		ServerIRC *server = cmd.getServer();

		if (cmd.getArgs().size() < 2) {
			client->recieveMessage(ERR_NEEDMOREPARAMS(std::string("OPER")));
			return false;
		}

		if (!server->isGoodPassword(cmd.getArg(1))) {
			client->recieveMessage(ERR_PASSWDMISMATCH);
			return false;
		}
		client->recieveMessage(RPL_YOUREOPER);
		client->setOperator(true);
		return true;
	}
}
