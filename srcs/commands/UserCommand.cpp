/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allanganoun <allanganoun@student.42lyon    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 16:44:28 by allanganoun       #+#    #+#             */
/*   Updated: 2022/05/16 19:34:32 by allanganoun      ###   ########lyon.fr   */
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
		//std::map<int, ClientIRC *>::iterator user = server->getClients().begin();
		//while (user != server->getClients().end())
		//{
		//	std::cout << "IN" << std::endl;
		//	std::cout<< "NICK = " << user->second->getNick() << std::endl;
		////if (args[0].compare(user->second->getNick()) == 0)
		////	{
		////		client->recieveMessage(ERR_ALREADYREGISTRED);
		////		return false;
		////	}
		//	user++;
		//	std::cout << "OUT" << std::endl;
		//}
		client->recieveMessage(RPL_WELCOME(args[0], args[2], args[1]));
		return true;
	}
}
