/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alganoun <alganoun@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 21:31:41 by tglory            #+#    #+#             */
/*   Updated: 2022/05/28 18:16:51 by alganoun         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/NickCommand.hpp"

namespace ft {

	NickCommand::NickCommand() : ClientCommand("NICK", 1, "Change your nickname", "<nickname>", false, false, false) {}

	NickCommand::~NickCommand() {}

	bool NickCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		ServerIRC *server = cmd.getServer();
		std::string oldNick = client->getNick();
		std::string newNick = cmd.getArg(0);
		std::stringstream ss;

		if (newNick.empty()) {
			ss << INFO << C_YELLOW << *client << " try to set a empty Nickname." << C_RESET << std::endl;
			logAndPrint(ss.str());
			client->recieveMessage(ERR_NONICKNAMEGIVEN);
			return false;
		}
		else if (newNick.find_first_not_of(std::string(LETTERS) 
										+ std::string(SPECIAL) 
										+ std::string(NUMBERS)) != std::string::npos){
			client->recieveMessage(ERR_ERRONEUSNICKNAME(newNick));
			return false;
		}
		if (server->getClientByNick(newNick)) {
			ss << INFO << C_YELLOW << *client << " try to set a nickname already used." << C_RESET << std::endl;
			logAndPrint(ss.str());
			client->recieveMessage(ERR_NICKNAMEINUSE(newNick));
			return false;
		}
		client->recieveMessage(RPL_NICK(newNick, client->getNick(), client->getUsername(), client->getHost()));
		for (std::vector<ChannelIRC*>::const_iterator it = server->getChannels().begin(); it != server->getChannels().end(); ++it) {
			ChannelIRC *channel = *it;
			if (clientExists(client, channel->getClientList()))
				channel->sendMessageToAll(client, cmd.getFullCmd());
		}
		client->setNick(newNick);
		ss << INFO << C_YELLOW << "Nick of ";
		if (oldNick.empty())
			ss << *client;
		else
			ss << oldNick;
		ss << " is now '" << newNick << "'." << C_RESET << std::endl;
		logAndPrint(ss.str());
		return true;
	}

}
