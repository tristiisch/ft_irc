/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 19:05:54 by tglory            #+#    #+#             */
/*   Updated: 2022/05/28 14:53:17 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/JoinCommand.hpp"
#include <sstream>

namespace ft {

	JoinCommand::JoinCommand() : ClientCommand("JOIN", 1, "Join a channel and create it if not exist", "<channel>", true, true, false) {}

	JoinCommand::~JoinCommand() {}

	bool	JoinCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		ServerIRC *server = cmd.getServer();
		std::vector<std::string> args = cmd.getArgs();
		std::vector<std::string> channels;
		std::stringstream ss;

		channels = split(args[0], ",");
		for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it) {
			ss << INFO << C_BLUE << "Client " << *client << " want to JOIN channel '" << *it << "'" << C_RESET << std::endl;
			logAndPrint(ss.str());
			ss.str("");
	
			if ((*it)[0] != '#') {
				client->recieveMessage(ERR_NOSUCHCHANNEL(*it));
				continue;
			}
			ChannelIRC *channel = server->getChannel(*it);
			if (!channel) {
				channel = new ChannelIRC(it->c_str(), client);
				server->addChannel(channel);
			} else {
				int ret = channel->addUser(client);
				if (ret < 0) {
					this->errorManagement(ret, client, (*it));
					continue;
				}
			}
			client->recieveMessage(":" + client->getNick() + " " + RPL_JOIN(*it));
			channel->sendMessageToAll(client, RPL_JOIN(*it));
			channel->userJoin(client);
		}
		return true;
	}

	void	JoinCommand::errorManagement(int ret, ClientIRC *const &client, std::string const &arg) const
	{
		if (ret == CHANNEL_FULL)
			client->recieveMessage(ERR_CHANNELISFULL(arg));
		else if (ret == USER_BANNED)
			client->recieveMessage(ERR_BANNEDFROMCHAN(arg));
		else if (ret == ALREADY_IN_CHANNEL)
			client->recieveMessage(ERR_ALREADYREGISTRED);
	}
}
