/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alganoun <alganoun@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 16:44:28 by allanganoun       #+#    #+#             */
/*   Updated: 2022/05/18 22:57:40 by alganoun         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/UserCommand.hpp"

namespace ft {

	UserCommand::UserCommand() : ClientCommand("USER") {}

	UserCommand::~UserCommand() {}

	bool UserCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		std::vector<std::string> args = cmd.getArgs();
		std::cout << args[0] + " " + args[1] + " " + args[2] + " " + args[3] << std::endl;
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
		client->recieveMessage(RPL_WELCOME(args[3], args[0],std::string("127.0.0.1")));
		client->recieveMessage(RPL_YOURHOST);
		return true;
	}
}
