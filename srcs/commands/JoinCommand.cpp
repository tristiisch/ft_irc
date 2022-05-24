/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 19:05:54 by tglory            #+#    #+#             */
/*   Updated: 2022/05/24 18:59:34 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/JoinCommand.hpp"

namespace ft {

	JoinCommand::JoinCommand() : ClientCommand("JOIN", true, false) {}

	JoinCommand::~JoinCommand() {}

	bool	JoinCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		ServerIRC *server = cmd.getServer();
		std::vector<std::string> args = cmd.getArgs();
		std::vector<std::string> channels;

		if (args.empty())
		{
			client->recieveMessage(ERR_NEEDMOREPARAMS(std::string("JOIN")));
			return false;
		}
		channels = split(args[0], ",");
		for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it) {
			std::cout << C_BLUE << "Client " << *client << " want to JOIN channel '" << *it << "'" << C_RESET << std::endl;
			if ((*it)[0] != '#') {
				client->recieveMessage(ERR_NOSUCHCHANNEL(*it));
				continue;
			}
			ChannelIRC *channel = server->getChannel(*it);
			if (!channel) {
				channel = new ChannelIRC(it->c_str());
				server->addChannel(channel);
			}
			int ret = channel->addUser(client);
			if (ret < 0) {
				ErrorManagement(ret, client, (*it));
				continue;
			}
			client->recieveMessage(":" + client->getNick() + " " + RPL_JOIN(*it));
			channel->sendMessageToAll(client, RPL_JOIN(*it));
			channel->channelRecap(client);
		}
		return true;
	}

	void	JoinCommand::ErrorManagement(int ret, ClientIRC *const &client, std::string const &arg) const
	{
		if (ret == CHANNEL_FULL)
			client->recieveMessage(ERR_CHANNELISFULL(arg));
		else if (ret == USER_BANNED)
			client->recieveMessage(ERR_BANNEDFROMCHAN(arg));
		else if (ret == ALREADY_IN_CHANNEL)
			client->recieveMessage(ERR_ALREADYREGISTRED);
	}
}
