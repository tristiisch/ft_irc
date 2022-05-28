/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NoticeCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 21:08:34 by alganoun          #+#    #+#             */
/*   Updated: 2022/05/28 15:20:32 by tglory           ###   ########lyon.fr   */
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

			ss << INFO << "Client " << *client << " want to send the NOTICE message " << "'" << args[1] << "'" << " in the channel " << args[0] << C_RESET << std::endl;
			logAndPrint(ss.str());

			if (!channel) {
				client->recieveMessage(ERR_CANNOTSENDTOCHAN(args[0]));
				return false;
			}
			channel->sendMessageToAll(client, cmd.getFullCmd());
		}
		else
		{
			ClientIRC *target  = server->getClientByNick(args[0]);

			ss << INFO << C_BLUE << "Client " << *client << " want to send the NOTICE message " << "'" << args[1] << "'" << " to " << args[0] << C_RESET << std::endl;
			logAndPrint(ss.str());

			if (!target) {
				client->recieveMessage(ERR_NOSUCHNICK(args[0]));
				return false;
			}
			client->sendMessage(target, cmd.getFullCmd());
		}
		return true;
	}
}
