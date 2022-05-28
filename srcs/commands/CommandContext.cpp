/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandContext.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 20:14:43 by tglory            #+#    #+#             */
/*   Updated: 2022/05/28 16:05:53 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/CommandContext.hpp"

namespace ft {
	CommandContext::CommandContext() {}
	CommandContext::CommandContext(ServerIRC *server, ClientIRC *client, std::string &fullcmd, std::string &cmd, std::vector<std::string> &args) : server(server), client(client),
		fullcmd(fullcmd), cmd(cmd), args(args) {}

	CommandContext &CommandContext::operator=(CommandContext const &instance) {
		this->server = instance.server;
		this->client = instance.client;
		this->fullcmd = instance.fullcmd;
		this->cmd = instance.cmd;
		this->args = instance.args;
		return *this;
	}

	CommandContext::~CommandContext() {}

	ServerIRC* CommandContext::getServer() {
		return this->server;
	}
	ClientIRC* CommandContext::getClient() {
		return this->client;
	}
	std::string& CommandContext::getFullCmd() {
		return this->fullcmd;
	}
	std::string& CommandContext::getCmd() {
		return this->cmd;
	}
	std::vector<std::string>& CommandContext::getArgs() {
		return this->args;
	}
	std::string& CommandContext::getArg(size_t index) {
		if (this->args.size() <= index)
			throw std::exception();
		return this->args.at(index);
	}
	std::string CommandContext::getFromArg(size_t index) {
		std::stringstream ss;

		if (this->args.size() <= index)
			throw std::exception();
		for (size_t i = index; i < this->args.size(); ++i) {
			ss << this->args[i];
			if (i + 1 != this->args.size()) {
				ss << ' ';
			}
		}
		return ss.str();
	}
}
