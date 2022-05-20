/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alganoun <alganoun@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 00:40:54 by tglory            #+#    #+#             */
/*   Updated: 2022/05/20 17:48:21 by alganoun         ###   ########lyon.fr   */
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
		if (args.size() > 2)
			client->recieveMessage(ERR_NEEDMOREPARAMS(std::string("KICK")));
		// ici il faut checker les arguments, il peut y avoir plusieurs personnes kicked en même temps sur plusieurs channel
		std::cout << C_BLUE << "Client " << *client << " want to Kick '" << args[1]
											<< "'" << "from the channel" <<args[0] <<  C_RESET << std::endl;
		ChannelIRC *channel  = server->getChannel(args[0]);
		if (channel) {
			if (channel->removeUser(client) == NO_SUCH_NICK)
				client->recieveMessage(ERR_USERNOTINCHANNEL(args[1], channel->getName()));
			channel->sendMessageToAll(client, cmd.getFullCmd());
		} else
			client->recieveMessage(ERR_NOSUCHCHANNEL(args[0]));
		return true;
	}
}