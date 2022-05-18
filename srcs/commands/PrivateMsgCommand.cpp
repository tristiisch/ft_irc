/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivateMsgCommand.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alganoun <alganoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 19:25:31 by alganoun          #+#    #+#             */
/*   Updated: 2022/05/18 18:34:48 by alganoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/PrivateMsgCommand.hpp"

namespace ft
{
	PrivateMsgCommand::PrivateMsgCommand() : ClientCommand("PRIVMSG", true, false) {}

	PrivateMsgCommand::~PrivateMsgCommand() {}

	bool PrivateMsgCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		ServerIRC *server = cmd.getServer();
		std::vector<std::string> args = cmd.getArgs();
		std::vector<std::string> channels = split(args[0], ","); // split channels separe by , in command received

		if (args[0][0] == '#')
		{
			std::cout << C_BLUE << "Client " << *client << " want to send the message "
										<< "'" <<args[1] << "'" << " in the channel " << args[0] << C_RESET << std::endl;
			ChannelIRC* channel  = server->getChannel(args[0]);
			if (!channel) {
				client->recieveMessage(ERR_CANNOTSENDTOCHAN(args[0]));
				return false;
			}
			channel->sendMessageToAll(client, cmd.getFullCmd());
		}
		else
		{
			std::cout << C_BLUE << "Client " << *client << " want to send the message "
										<< "'" <<args[1] << "'" << " to " << args[0] << C_RESET << std::endl;
			ClientIRC *target  = server->getClientByNick(args[0]);
			if (!client) {
				client->recieveMessage(ERR_NOSUCHNICK(args[0]));
				return false;
			}
			client->sendMessage(target, cmd.getFullCmd());
		}
		return true;
	}
}
