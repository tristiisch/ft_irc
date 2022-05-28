/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 16:44:28 by allanganoun       #+#    #+#             */
/*   Updated: 2022/05/28 14:56:17 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/UserCommand.hpp"

namespace ft {

	UserCommand::UserCommand() : ClientCommand("USER", 4, "Login into server", "<nickname> 0 * <realname>", true, false, false) {}

	UserCommand::~UserCommand() {}

	bool UserCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		std::vector<std::string> args = cmd.getArgs();

		if (client->isRegistered())
		{
			client->recieveMessage(ERR_ALREADYREGISTRED);
			return false;
		}
		client->setRegistered(true);
		client->setUsername(args[2]);
		client->recieveMessage(RPL_WELCOME(client->getNick(), client->getUsername(), client->getHost()));
		return true;
	}
}
