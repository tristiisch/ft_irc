/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PartCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 20:35:09 by tglory            #+#    #+#             */
/*   Updated: 2022/05/28 14:34:57 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/PartCommand.hpp"

namespace ft {

	PartCommand::PartCommand() : ClientCommand("PART", 1, "Leave a channel", "<channel> :<msg>", true, false) {}

	PartCommand::~PartCommand() {}

	bool PartCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		ServerIRC *server = cmd.getServer();
		std::vector<std::string> args = cmd.getArgs();
		std::vector<std::string> channels = split(args[0], ",");

		for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it) {
			std::cout << C_BLUE << "Client " << *client << " want to PART channel '" << *it << "'" << C_RESET << std::endl;
			std::string str;

			if ((*it).empty() || (*it)[0] == ':')
				str = "*";
			else
				str = *it;

			ChannelIRC *channel = server->getChannel(str);
			if (!channel) {
				client->recieveMessage(ERR_NOSUCHCHANNEL(str));
				continue;
			}
			if (!ft::clientExists(client, channel->getClientList())) {
				client->recieveMessage(ERR_NOTONCHANNEL(str));
				continue;
			}
			client->sendMessage(client, cmd.getFullCmd());
			channel->sendMessageToAll(client, cmd.getFullCmd());
			channel->clearUser(client);
		}
		return true;
	}
}
