/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 19:05:54 by tglory            #+#    #+#             */
/*   Updated: 2022/05/18 16:21:20 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/JoinCommand.hpp"

namespace ft {

	JoinCommand::JoinCommand() : ClientCommand("JOIN", true, false) {}

	JoinCommand::~JoinCommand() {}

	bool JoinCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		ServerIRC *server = cmd.getServer();
		std::vector<std::string> args = cmd.getArgs();
		std::vector<std::string> channels = split(args[0], ",");

		if (args.size() != 1)
		{
			client->recieveMessage(ERR_NEEDMOREPARAMS(std::string("JOIN")));
			return false;
		}
		for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it) {
			std::cout << C_BLUE << "Client " << *client << " want to JOIN channel '" << *it << "'" << C_RESET << std::endl;

			// Si le channel n'existe pas on le crée sinon on ajoute le client channel correspondant
			ChannelIRC *channel = server->getChannel(*it);
			if (channel) {
				if (channel->addUser(client) == CHANNEL_FULL) {
					client->recieveMessage(ERR_CHANNELISFULL(*it));
					continue;
				}
				client->recieveMessage(RPL_JOIN(client->getNick(), *it));
				channel->sendMessageToAll(client, RPL_JOIN(client->getNick(), *it));
			} else {
				ChannelIRC *new_channel = new ChannelIRC(it->c_str(), client);
				server->addChannel(new_channel);
				client->recieveMessage(RPL_JOIN(client->getNick(), *it));
			}
		}
		return true;
	}
}
