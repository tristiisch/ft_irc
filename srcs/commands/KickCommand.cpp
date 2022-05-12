/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alganoun <alganoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 19:07:51 by alganoun          #+#    #+#             */
/*   Updated: 2022/05/12 19:26:38 by alganoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/KickCommand.hpp"
	
namespace ft
{
	KickCommand::KickCommand() : ClientCommand("KICK") {}

	KickCommand::~KickCommand() {}

	void KickCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		ServerIRC *server = cmd.getServer();
		std::vector<std::string> args = cmd.getArgs();

		std::cout << C_BLUE << "Client " << *client << " want to Kick '" << args[1]
											<< "'" << "from the channel" <<args[0] <<  C_RESET << std::endl;

		// Si le channel n'existe pas on le crée sinon on ajoute le client channel correspondant
		std::vector<ChannelIRC>::iterator channel  = server->getChannels().begin();
		while(channel != server->getChannels().end())
		{
			if (channel->getName() == args[0])
			{	
				channel->removeUser(client);
				channel->sendMessageToAll(client, cmd.getFullCmd());
				return;
			}
			channel++;
		}
		client->recieveMessage(ERR_NOSUCHCHANNEL(args[0]));
	}
}