/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 19:05:54 by tglory            #+#    #+#             */
/*   Updated: 2022/05/25 18:07:15 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/QuitCommand.hpp"

namespace ft {

	QuitCommand::QuitCommand() : ClientCommand("QUIT") {}

	QuitCommand::~QuitCommand() {}

	bool QuitCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		ServerIRC *server = cmd.getServer();
		std::vector<std::string> args = cmd.getArgs();
		std::stringstream ss;

		ss << INFO << "Client " << *client << " disconnect with '" << join(args, " ") << "'" << C_RESET << std::endl;
		logAndPrint(ss.str());
		server->deleteClient(client);
		return true;
	}

}
