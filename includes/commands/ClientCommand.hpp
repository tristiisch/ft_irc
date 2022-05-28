/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientCommand.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 18:53:37 by tglory            #+#    #+#             */
/*   Updated: 2022/05/25 19:23:36 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../ft_irc.hpp"
#include "../ClientIRC.hpp"
#include "../ServerIRC.hpp"
#include "CommandContext.hpp"
namespace ft {
	class ClientCommand {

		private:
			ClientCommand();

		protected:
			std::string name;
			int minArg;
			std::string description;
			std::string format;
			bool needToBeAuthorized;
			bool needToBeOperator;

		public :
			ClientCommand(std::string name);
			ClientCommand(std::string name, int minArg);
			ClientCommand(std::string name, int minArg, std::string description, std::string format);
			ClientCommand(std::string name, int minArg, std::string description, std::string format, bool needToBeAuthorized, bool needToBeOperator);
			
			ClientCommand &operator=(ClientCommand const &instance);

			virtual ~ClientCommand() {};

			virtual bool execute(CommandContext &cmd) const = 0;

			const std::string& getName() const;
			const std::string& getDescription() const;
			const std::string& getFormat() const;
			const bool& isNeededToBeAutorized() const;
			const bool& isNeededToBeOperator() const;
			const int& getMinArg() const;
	};
}
