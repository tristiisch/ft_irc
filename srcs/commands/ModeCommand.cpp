/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/22 20:33:40 by alganoun          #+#    #+#             */
/*   Updated: 2022/05/23 18:34:50 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/ModeCommand.hpp"

namespace ft {

	ModeCommand::ModeCommand() : ClientCommand("MODE") {}

	ModeCommand::~ModeCommand() {}

	bool ModeCommand::execute(CommandContext &cmd) const {
		ClientIRC 	*client = cmd.getClient();
		ServerIRC 	*server = cmd.getServer();
		ClientIRC 	*target;
		std::vector<std::string> args = cmd.getArgs();
		int ret = 0;
		if (args.size() < 2){
			client->recieveMessage(ERR_NEEDMOREPARAMS(std::string("MODE")));
			return false;
		}
		std::cout << "An Mode command is used by " << client->getNick() << std::endl;
		if (client->isOperator() == false){
			client->recieveMessage(ERR_NOPRIVILEGES);
			return false;
		}
		if (args[1].size() == 2 && (args[1][0] == '+' || args[1][0] == '-'))
		{
			if (args[0][0] == '#'){
				if (args.size() < 3){
					client->recieveMessage(ERR_NEEDMOREPARAMS(std::string("MODE")));
					return false;
				}
				ChannelIRC *channel = server->getChannel(args[0]);
				target = server->getClientByNick(args[2]);
				if (channel == NULL){
					client->recieveMessage(ERR_NOSUCHCHANNEL(args[0]));
					return false;
				}
				else if (target == NULL){
					client->recieveMessage(ERR_NOSUCHNICK(args[2]));
					return false;
				}
				else if (args[1][1] == 'o' && !clientExists(client, channel->getClientList())){
					client->recieveMessage(ERR_NOTONCHANNEL(channel->getName()));
					return false;
				}
				if (!args[1].compare("+o")){
					ret = channel->addOperator(target);
					if (ret < 0){
						ErrorManagement(ret, target, args[0]);
						return false;
					}
					else{
						client->recieveMessage(":" + args[2] + " " + cmd.getFullCmd());
						channel->sendMessageToAll(client, cmd.getFullCmd());
						return true;
					}
				}
				else if (!args[1].compare("-o")){
					if (channel->removeOperator(target) == NO_SUCH_NICK){
						client->recieveMessage(ERR_NOSUCHNICK(args[0]));
						return false;
					}
					else{
						client->recieveMessage(":" + args[2] + " " + cmd.getFullCmd());
						channel->sendMessageToAll(client, cmd.getFullCmd());
						return true;
					}
				}
				else if (!args[1].compare("+b")){
					if (channel->addBannedUser(target) == ALREADY_BANNED)
						return false;
					else{
						client->recieveMessage(cmd.getFullCmd());
						channel->sendMessageToAll(client, cmd.getFullCmd());
						return true;
					}
				}
				else if (!args[1].compare("-b")){
					if (channel->unbanUser(target) == NO_SUCH_NICK){	
						client->recieveMessage(ERR_NOSUCHNICK(args[2]));
						return false;
					}
					else{
						client->recieveMessage(cmd.getFullCmd());
						channel->sendMessageToAll(client, cmd.getFullCmd());
						return true;
					}
				}	
			}
			else if (!args[0].compare(client->getNick())){
				client->recieveMessage(ERR_USERSDONTMATCH);
				return false;
			}
		}
		client->recieveMessage(ERR_UMODEUNKNOWNFLAG);
		return false;
	}

	
	void	ModeCommand::ErrorManagement(int ret, ClientIRC *const &client, std::string const &arg) const
	{
		if (ret == ALREADY_OPERATOR)
			return;
		else if (ret == NO_SUCH_NICK)
			client->recieveMessage(ERR_USERNOTINCHANNEL(client->getNick(), arg));
	}
}