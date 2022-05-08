/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CapCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 19:05:54 by tglory            #+#    #+#             */
/*   Updated: 2022/05/08 20:24:00 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/CapCommand.hpp"

namespace ft {

	CapCommand::CapCommand() : ClientCommand("CAP") {}

	CapCommand::~CapCommand() {}

	void CapCommand::execute(ServerIRC &server, ClientIRC *client, std::string &cmd, std::string &args) const {
		(void)cmd;
		std::cout << C_BLUE << "Client CAP " << args << C_RESET << std::endl;
	}

}
