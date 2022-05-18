/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandContext.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 02:16:49 by tglory            #+#    #+#             */
/*   Updated: 2022/05/17 02:09:00 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../ServerIRC.hpp"
#include "../ClientIRC.hpp"
#include "ClientCommand.hpp"
#include <exception>
#include <sstream>

namespace ft {

	class CommandContext {

		private:
			ServerIRC *server;
			ClientIRC *client;
			std::string fullcmd;
			std::string cmd;
			std::vector<std::string> args;

		public:
			CommandContext();
			CommandContext(ServerIRC *server, ClientIRC *client, std::string &fullcmd, std::string &cmd, std::vector<std::string> &args) : server(server), client(client),
				fullcmd(fullcmd), cmd(cmd), args(args) {}

			CommandContext &operator=(CommandContext const &instance) {
				this->server = instance.server;
				this->client = instance.client;
				this->fullcmd = instance.fullcmd;
				this->cmd = instance.cmd;
				this->args = instance.args;
				return *this;
			}
			~CommandContext() {}

			ServerIRC* getServer() {
				return this->server;
			}
			ClientIRC* getClient() {
				return this->client;
			}
			std::string& getFullCmd() {
				return this->fullcmd;
			}
			std::string& getCmd() {
				return this->cmd;
			}
			std::vector<std::string>& getArgs() {
				return this->args;
			}
			std::string& getArg(size_t index) {
				if (this->args.size() <= index)
					throw new std::exception();
				return this->args.at(index);
			}
			std::string getFromArg(size_t index) {
				std::stringstream ss;

				if (this->args.size() <= index)
					throw new std::exception();
				for (size_t i = index; i < this->args.size(); ++i) {
					ss << this->args[i];
					if (i + 1 != this->args.size()) {
						ss << ' ';
					}
				}
				return ss.str();
			}
	};
}
