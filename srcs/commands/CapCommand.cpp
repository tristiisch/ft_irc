/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CapCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 19:05:54 by tglory            #+#    #+#             */
/*   Updated: 2022/05/12 18:13:18 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/CapCommand.hpp"
#include <iterator>
#include <sstream>

namespace ft {

	CapCommand::CapCommand() : ClientCommand("CAP") {}

	CapCommand::~CapCommand() {}

	void CapCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		// ServerIRC *server = cmd.getServer();
		std::vector<std::string> args = cmd.getArgs();

		std::cout << C_YELLOW << *client << " set the client CAP at '" << join(args, " ") << "'" << C_RESET << std::endl;
		if (args[0] == "LR")
			client->setDelimitator("\n");
	}

}
