/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 00:40:54 by tglory            #+#    #+#             */
/*   Updated: 2022/05/28 14:37:30 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/KickCommand.hpp"
	
namespace ft
{
	KickCommand::KickCommand() : ClientCommand("KICK", 2, "Kick a client from a channel", "<channel> <nickname>", true, false) {}

	KickCommand::~KickCommand() {}


	bool KickCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		ServerIRC *server = cmd.getServer();
		std::vector<std::string> args = cmd.getArgs();
		std::vector<std::string> channels_args = split(args[0], ",");
		std::vector<std::string> users_args= split(args[1], ",");
		std::stringstream ss;

		if (channels_args.empty() || users_args.empty()) {
			client->recieveMessage(ERR_NEEDMOREPARAMS(std::string(this->name)));
			return false;
		}
		
		ss << INFO << C_BLUE << "Client " << *client << " want to Kick '" << args[1] << "'" << " from the channel" << args[0] << C_RESET << std::endl;
		logAndPrint(ss.str());
		ss.str("");
		
		for (std::vector<std::string>::iterator itUsers = users_args.begin(); itUsers != users_args.end(); ++itUsers)
		{
			for (std::vector<std::string>::iterator itChannel = channels_args.begin(); itChannel != channels_args.end(); ++itChannel)
			{
				ChannelIRC *channel  = server->getChannel(*itChannel);
				ClientIRC *target = server->getClientByNick(*itUsers);
				if (!channel) {
					client->recieveMessage(ERR_NOSUCHCHANNEL(args[0]));
					continue;
				} 
				if (!clientExists(client, channel->getClientList())) {
					client->recieveMessage(ERR_NOTONCHANNEL(channel->getName()));
					continue;
				}
				if (!clientExists(client, channel->getOpeList())) {
					client->recieveMessage(ERR_CHANOPRIVSNEEDED(channel->getName()));
					continue;
				}
				if (!clientExists(target, channel->getClientList())) {
					ss << INFO << "The User " << target->getNick() << " does not exist in channel " << channel->getName() << "." << std::endl;
					logAndPrint(ss.str());
					ss.str("");
					client->recieveMessage(ERR_USERNOTINCHANNEL(*itUsers, channel->getName()));
					continue;
				}
				channel->sendMessageToAll(client, cmd.getFullCmd()); // il faut peut être revoir ça
				channel->removeUser(target);
				client->sendMessage(client, cmd.getFullCmd());
			}
		}
		return true;
	}
}
