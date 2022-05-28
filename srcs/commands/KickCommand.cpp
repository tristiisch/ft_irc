/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alganoun <alganoun@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 00:40:54 by tglory            #+#    #+#             */
/*   Updated: 2022/05/28 13:15:09 by alganoun         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/KickCommand.hpp"
	
namespace ft
{
	KickCommand::KickCommand() : ClientCommand("KICK", true, false) {}

	KickCommand::~KickCommand() {}


	bool KickCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		ServerIRC *server = cmd.getServer();
		std::vector<std::string> args = cmd.getArgs();
		std::vector<std::string> channels_args = split(args[0], ",");
		std::vector<std::string> users_args= split(args[1], ",");
		if (args.size() < 2) {
			client->recieveMessage(ERR_NEEDMOREPARAMS(std::string("KICK")));
			return false;
		}
		
		std::cout << C_BLUE << "Client " << *client << " want to Kick '" << args[1]
											<< "'" << " from the channel" <<args[0] <<  C_RESET << std::endl;
		
		std::vector<std::string>::iterator users = users_args.begin();
		std::vector<std::string>::iterator channels = channels_args.begin();
		
		while (users != users_args.end())
		{
			while (channels != channels_args.end())
			{
				ChannelIRC *channel  = server->getChannel(*channels);
				ClientIRC *target = server->getClientByNick(*users);
				if (channel) {
					if (!clientExists(client, channel->getClientList())){
						client->recieveMessage(ERR_NOTONCHANNEL(channel->getName()));
						channels++;
						continue;
					}
					else if (!clientExists(client, channel->getOpeList())){
						client->recieveMessage(ERR_CHANOPRIVSNEEDED(channel->getName()));
						channels++;
						continue;
					}
					else if (clientExists(target, channel->getClientList()) == false)
					{
						std::cout << "The User " << target->getNick() << " does not exist in this channel." << std::endl;
						client->recieveMessage(ERR_USERNOTINCHANNEL(*users, channel->getName()));
						channels++;
						continue;
					}
					channel->sendMessageToAll(client, cmd.getFullCmd()); // il faut peut être revoir ça
					channel->removeUser(target);
					client->sendMessage(client, cmd.getFullCmd());
				} else
					client->recieveMessage(ERR_NOSUCHCHANNEL(args[0]));
				channels++;
			}
			users++;
		}
		return true;
	}

	
}