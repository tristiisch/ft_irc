/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allanganoun <allanganoun@student.42lyon    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 16:44:28 by allanganoun       #+#    #+#             */
/*   Updated: 2022/05/15 19:18:51 by allanganoun      ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/UserCommand.hpp"

namespace ft {

	UserCommand::UserCommand() : ClientCommand("USER") {}

	UserCommand::~UserCommand() {}

	bool UserCommand::execute(CommandContext &cmd) const {
		std::string command = "USER";
		ClientIRC *client = cmd.getClient();
		ServerIRC *server = cmd.getServer();
		std::vector<std::string> args = cmd.getArgs();
		if (args.size() != 4)
		{
			client->recieveMessage(ERR_NEEDMOREPARAMS(command));
			return false;
		}
		std::map<int, ClientIRC *>::iterator user = server->getClients().begin();
		while (user != server->getClients().end())
		{
			if (args[0].compare(user->second->getNick()) == 0)
			{
				client->recieveMessage(ERR_ALREADYREGISTRED);
				return false;
			}
			user++;
		}
		client->recieveMessage(RPL_WELCOME(args[0], args[2], args[1]));
		return true;
	}
}
