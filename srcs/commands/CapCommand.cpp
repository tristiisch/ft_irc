/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CapCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 19:05:54 by tglory            #+#    #+#             */
/*   Updated: 2022/05/28 14:51:58 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/CapCommand.hpp"

namespace ft {

	CapCommand::CapCommand() : ClientCommand("CAP", 1, "Define the trame delimiter", "<LS|CRLF>", false, false, false) {}

	CapCommand::~CapCommand() {}

	bool CapCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		std::string delimiterName;
		std::string arg = cmd.getArg(0);

		if (arg.rfind("LS", 0) == 0) {
			delimiterName = "LS";
			client->setDelimitator(LF_DELIMITER);
		} else if (arg.rfind("CRLF", 0) == 0) {
			delimiterName = "CRLF";
			client->setDelimitator(CRLF_DELIMITER);
		} else {
			std::cout << C_RED << *client << " try to set UNKNOWN string delimiter (was '" << join(cmd.getArgs(), " ") << "')" << C_RESET << std::endl;
		 	return false;
		}
		std::cout << C_YELLOW << *client << " set string delimiter at '" << delimiterName << "'" << C_RESET << std::endl;
		return true;
	}
}
