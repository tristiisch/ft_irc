/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allanganoun <allanganoun@student.42lyon    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 19:05:54 by tglory            #+#    #+#             */
/*   Updated: 2022/05/18 14:04:01 by allanganoun      ###   ########lyon.fr   */
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
		if (args.size() != 1)
		{
			client->recieveMessage(ERR_NEEDMOREPARAMS(std::string("JOIN")));
			return false;
		}
		std::cout << C_BLUE << "Client " << *client << " want to JOIN channel '" << args[0] << "'" << C_RESET << std::endl;
		std::vector<ChannelIRC>::iterator channel  = server->getChannels().begin();
		while(channel != server->getChannels().end())
		{
			if (channel->getName() == args[0])
			{
				if (channel->addUser(client) == CHANNEL_FULL)
					client->recieveMessage(ERR_CHANNELISFULL(args[0]));
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
