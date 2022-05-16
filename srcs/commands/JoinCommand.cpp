/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allanganoun <allanganoun@student.42lyon    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 19:05:54 by tglory            #+#    #+#             */
/*   Updated: 2022/05/16 23:31:06 by allanganoun      ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/JoinCommand.hpp"

namespace ft {

	JoinCommand::JoinCommand() : ClientCommand("JOIN") {}

	JoinCommand::~JoinCommand() {}

	bool JoinCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		ServerIRC *server = cmd.getServer();
		std::vector<std::string> args = cmd.getArgs();

		std::cout << C_BLUE << "Client " << *client << " want to JOIN channel '" << args[0] << "'" << C_RESET << std::endl;

		// Si le channel n'existe pas on le crée sinon on ajoute le client channel correspondant
		std::vector<ChannelIRC>::iterator channel  = server->getChannels().begin();
		while(channel != server->getChannels().end())
		{
			if (channel->getName() == args[0])
			{
				channel->addUser(client);
				client->recieveMessage(RPL_JOIN(client->getNick(), args[0]));
				channel->sendMessageToAll(client, RPL_JOIN(client->getNick(), args[0]));
				return true;
			}
			channel++;
		}
		ChannelIRC new_channel(args[0].c_str(), client);
		server->getChannels().push_back(new_channel);
		client->recieveMessage(RPL_JOIN(client->getNick(), args[0]));
		return true;
	}

}
