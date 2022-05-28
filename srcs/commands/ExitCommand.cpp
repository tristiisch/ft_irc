/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExitCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alganoun <alganoun@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 19:05:54 by tglory            #+#    #+#             */
/*   Updated: 2022/05/28 15:51:30 by alganoun         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/ExitCommand.hpp"

namespace ft {

	ExitCommand::ExitCommand() : ClientCommand("EXIT", 0, "\\!/ Kill the server", "", true, true) {}

	ExitCommand::~ExitCommand() {}

	bool ExitCommand::execute(CommandContext &cmd) const {
		ServerIRC *server = cmd.getServer();
		ClientIRC *client = cmd.getClient();
		
		client->recieveMessage("006 " + client->getNick() + " Bye bye !");
		server->stop();
		return true;
	}
}
