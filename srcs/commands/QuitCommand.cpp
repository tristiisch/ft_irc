/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 19:05:54 by tglory            #+#    #+#             */
/*   Updated: 2022/05/08 19:47:51 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/QuitCommand.hpp"

namespace ft {

	QuitCommand::QuitCommand() : ClientCommand("QUIT") {}

	QuitCommand::~QuitCommand() {}


	void QuitCommand::execute(ServerIRC &server, ClientIRC *client, std::string &cmd, std::string &args) const {
		(void)cmd;
		(void)args;
		std::cout << C_BLUE << "Client " << client->getSocket() << " disconnect." << C_RESET << std::endl;
		server.deleteClient(client);
		std::cout << C_BLUE << "Client" << " delete." << C_RESET << std::endl;
	}

}
