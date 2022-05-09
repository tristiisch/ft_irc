/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 19:05:54 by tglory            #+#    #+#             */
/*   Updated: 2022/05/09 02:43:43 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/QuitCommand.hpp"

namespace ft {

	QuitCommand::QuitCommand() : ClientCommand("QUIT") {}

	QuitCommand::~QuitCommand() {}

	void QuitCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		ServerIRC server = cmd.getServer();
		std::vector<std::string> args = cmd.getArgs();

		std::cout << C_BLUE << "Client " << *client << " disconnect with '" << join(args, " ") << "'" << C_RESET << std::endl;
		server.deleteClient(client);
	}

}
