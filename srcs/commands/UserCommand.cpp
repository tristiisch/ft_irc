/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 16:44:28 by allanganoun       #+#    #+#             */
/*   Updated: 2022/05/25 18:22:30 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/UserCommand.hpp"

namespace ft {

	UserCommand::UserCommand() : ClientCommand("USER") {}

	UserCommand::~UserCommand() {}

	bool UserCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		std::vector<std::string> args = cmd.getArgs();

		if (args.size() != 4)
		{
			client->recieveMessage(ERR_NEEDMOREPARAMS(this->name));
			return false;
		}
		if (client->isRegistered())
		{
			client->recieveMessage(ERR_ALREADYREGISTRED);
			return false;
		}
		client->setRegistered(true);
		client->setUsername(args[2]);
		client->recieveMessage(RPL_WELCOME(client->getNick(), args[0], client->getHost()));
		return true;
	}
}
