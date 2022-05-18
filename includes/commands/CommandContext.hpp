/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandContext.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 02:16:49 by tglory            #+#    #+#             */
/*   Updated: 2022/05/18 20:30:47 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../ServerIRC.hpp"
#include "../ClientIRC.hpp"

namespace ft {

	class CommandContext {

		private:
			ServerIRC *server;
			ClientIRC *client;
			std::string fullcmd;
			std::string cmd;
			std::vector<std::string> args;

			CommandContext();
		public:
			CommandContext(ServerIRC *server, ClientIRC *client, std::string &fullcmd, std::string &cmd, std::vector<std::string> &args);

			CommandContext &operator=(CommandContext const &instance);
			~CommandContext();

			ServerIRC* getServer();
			ClientIRC* getClient();
			std::string& getFullCmd();
			std::string& getCmd();
			std::vector<std::string>& getArgs();
			std::string& getArg(size_t index);
			std::string getFromArg(size_t index);
	};
}
