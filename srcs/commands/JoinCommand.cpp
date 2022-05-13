/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 19:05:54 by tglory            #+#    #+#             */
/*   Updated: 2022/05/13 04:23:44 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/JoinCommand.hpp"

namespace ft {

	JoinCommand::JoinCommand() : ClientCommand("JOIN") {}

	JoinCommand::~JoinCommand() {}

	// In Dev
	bool JoinCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		// ServerIRC *server = cmd.getServer();
		std::vector<std::string> args = cmd.getArgs();

		std::cout << C_BLUE << "Client " << *client << " want to JOIN channel '" << args[0] << "'" << C_RESET << std::endl;
		return true;
	}

}
