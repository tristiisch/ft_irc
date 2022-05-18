/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CapCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 19:05:54 by tglory            #+#    #+#             */
/*   Updated: 2022/05/18 20:27:11 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/CapCommand.hpp"

namespace ft {

	CapCommand::CapCommand() : ClientCommand("CAP") {}

	CapCommand::~CapCommand() {}

	bool CapCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		std::string delimiterName;
		// ServerIRC *server = cmd.getServer();
		std::vector<std::string> args = cmd.getArgs();

		if (args[0].rfind("LS", 0) == 0) {
			delimiterName = "LS";
			client->setDelimitator(LF_DELIMITER);
		} else if (args[0].rfind("CRLF", 0) == 0) {
			delimiterName = "CRLF";
			client->setDelimitator(CRLF_DELIMITER);
		} else {
			std::cout << C_RED << *client << " try to set UNKNOWN string delimiter (was '" << join(args, " ") << "')" << C_RESET << std::endl;
		 	return false;
		}
		std::cout << C_YELLOW << *client << " set string delimiter at '" << delimiterName << "'" << C_RESET << std::endl;
		return true;
	}
}
