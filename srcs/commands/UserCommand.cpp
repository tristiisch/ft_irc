/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 16:44:28 by allanganoun       #+#    #+#             */
/*   Updated: 2022/05/18 02:38:45 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/UserCommand.hpp"

namespace ft {

	UserCommand::UserCommand() : ClientCommand("USER") {}

	UserCommand::~UserCommand() {}

	bool UserCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		ServerIRC *server = cmd.getServer();
		std::vector<std::string> args = cmd.getArgs();
		if (args.size() != 4)
		{
			client->recieveMessage(ERR_NEEDMOREPARAMS(this->name));
			return false;
		}
		std::map<int, ClientIRC *>::const_iterator user = server->getClients().find(client->getSocket());
		if (user != server->getClients().end())
		{
			client->recieveMessage(ERR_ALREADYREGISTRED);
			return false;
		}
		client->recieveMessage(RPL_WELCOME(args[0], args[2], args[1]));
		return true;
	}
}
