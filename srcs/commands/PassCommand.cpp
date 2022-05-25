/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 21:35:35 by tglory            #+#    #+#             */
/*   Updated: 2022/05/25 18:09:25 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/PassCommand.hpp"

namespace ft {

	PassCommand::PassCommand() : ClientCommand("PASS") {}

	PassCommand::~PassCommand() {}

	bool PassCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		ServerIRC *server = cmd.getServer();
		std::vector<std::string> args = cmd.getArgs();
		std::string clientPassword;
		std::stringstream ss;

		if (args.size() >= 1)
			clientPassword = args.at(0);

		if (!server->isGoodPassword(clientPassword)) {
			ss << INFO << C_RED << "Password '" << clientPassword << "' send by " << *client << " is not server password." << C_RESET << std::endl;
			logAndPrint(ss.str());
			return false;
		}
		client->setAuthorized(true);
		ss << INFO << C_YELLOW << "Client " << *client << " has enter good server password." << C_RESET << std::endl;
		logAndPrint(ss.str());
		return true;
	}
}
