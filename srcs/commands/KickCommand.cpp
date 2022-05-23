/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 00:40:54 by tglory            #+#    #+#             */
/*   Updated: 2022/05/23 18:03:16 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/KickCommand.hpp"
	
namespace ft
{
	KickCommand::KickCommand() : ClientCommand("KICK") {}

	KickCommand::~KickCommand() {}


	bool KickCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		ServerIRC *server = cmd.getServer();
		std::vector<std::string> args = cmd.getArgs();
		std::vector<std::string> channels_args = split(args[0], ",");
		std::vector<std::string> users_args= split(args[1], ",");
		if (args.size() < 2)
			client->recieveMessage(ERR_NEEDMOREPARAMS(std::string("KICK")));
		
		std::cout << C_BLUE << "Client " << *client << " want to Kick '" << args[1]
											<< "'" << "from the channel" <<args[0] <<  C_RESET << std::endl;
		
		std::vector<std::string>::iterator users = users_args.begin();
		std::vector<std::string>::iterator channels = channels_args.begin();
		
		while (users != users_args.end())
		{
			while (channels != channels_args.end())
			{
				ChannelIRC *channel  = server->getChannel(*channels);
				ClientIRC *user_client = server->getClientByNick(*users);
				if (channel) {
					if (!clientExists(client, channel->getClientList())){
						client->recieveMessage(ERR_NOTONCHANNEL(channel->getName()));
						return false;
					}
					else if (!clientExists(client, channel->getOpeList())){
						client->recieveMessage(ERR_CHANOPRIVSNEEDED(channel->getName()));
						return false;
					}
					else if (channel->removeUser(user_client) == NO_SUCH_NICK){
						client->recieveMessage(ERR_USERNOTINCHANNEL(*users, channel->getName()));
						return false;
					}
					channel->sendMessageToAll(client, cmd.getFullCmd()); // il faut peut être revoir ça 
				} else
					client->recieveMessage(ERR_NOSUCHCHANNEL(args[0]));
				channels++;
			}
			users++;
		}
		return true;
	}

	
}