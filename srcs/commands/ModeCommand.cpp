/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alganoun <alganoun@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/22 20:33:40 by alganoun          #+#    #+#             */
/*   Updated: 2022/05/22 20:35:15 by alganoun         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/ModeCommand.hpp"

namespace ft {

	ModeCommand::ModeCommand() : ClientCommand("MODE") {}

	ModeCommand::~ModeCommand() {}

	bool ModeCommand::execute(CommandContext &cmd) const {
		ClientIRC *client = cmd.getClient();
		ServerIRC *server = cmd.getServer();
		std::vector<std::string> args = cmd.getArgs();
		if (args.size() != 2){
			client->recieveMessage(ERR_NEEDMOREPARAMS(std::string("MODE")));
			return false;
		}
		std::cout << "An Mode command is used by " << client->getNick() << std::endl;
		if (args[1].compare("+o")){

		}
		else if (args[1].compare("-o")){

		}
		else if (args[1].compare("+b")){
		
		}
		else if (args[1].compare("-b")){

		}
		
		else{
			client->recieveMessage(ERR_UMODEUNKNOWNFLAG);
			return false;
		}

	}
}