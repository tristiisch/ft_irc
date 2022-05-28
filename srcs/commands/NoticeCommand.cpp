/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NoticeCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 21:08:34 by alganoun          #+#    #+#             */
/*   Updated: 2022/05/28 14:54:19 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/NoticeCommand.hpp"
	
namespace ft
{
	NoticeCommand::NoticeCommand() : ClientCommand("NOTICE", 2, "Same as PRIVMSG without auto reply", "<channel> :<msg>", true, true, false) {}

	NoticeCommand::~NoticeCommand() {}

	bool NoticeCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		ServerIRC *server = cmd.getServer();
		std::vector<std::string> args = cmd.getArgs();
		std::stringstream ss;

		if (args[0][0] == '#')
		{
			ChannelIRC* channel = server->getChannel(args[0]);

			ss << INFO << "Client " << *client << " want to send the message " << "'" << args[1] << "'" << " in the channel " << args[0] << C_RESET << std::endl;
			logAndPrint(ss.str());

			if (channel != NULL)
				channel->sendMessageToAll(client, cmd.getFullCmd());
			else
				client->recieveMessage(ERR_NOSUCHCHANNEL(args[0]));
		}
		else
		{

			ss << INFO << "Client " << *client << " want to send the message " << "'" << args[1] << "'" << " to " << args[0] << C_RESET << std::endl;
			logAndPrint(ss.str());

			for (std::map<int , ClientIRC *>::const_iterator it = server->getClients().begin(); it != server->getClients().end(); ++it)
			{
				if (it->second->getNick() == args[0])
				{	
					client->sendMessage(it->second, cmd.getFullCmd());
					return true;
				}
			}
			client->recieveMessage(ERR_NOSUCHNICK(args[0]));
		}
		return true;
	}
}