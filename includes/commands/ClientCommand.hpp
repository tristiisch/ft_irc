/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientCommand.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 18:53:37 by tglory            #+#    #+#             */
/*   Updated: 2022/05/09 02:36:22 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../ft_irc.hpp"
#include "../ClientIRC.hpp"
#include "../ServerIRC.hpp"
#include "../commands/CommandContext.hpp"
namespace ft {
	class ClientCommand {

		protected:
			std::string name;
			bool needToBeAuthorized;
			bool needToBeOperator;

		public :
			ClientCommand(std::string name) : name(name), needToBeAuthorized(false), needToBeOperator(false) {}

			ClientCommand(std::string name, bool needToBeAuthorized, bool needToBeOperator) : name(name),
				needToBeAuthorized(needToBeAuthorized), needToBeOperator(needToBeOperator) {}
			
			ClientCommand &operator=(ClientCommand const &instance) {
				this->name = instance.name;
				this->needToBeAuthorized = instance.needToBeAuthorized;
				this->needToBeOperator = instance.needToBeOperator;
				return *this;
			}

			virtual ~ClientCommand() {};

			// void virtual execute(CommandContext &cmd) const = 0;

			void virtual execute(CommandContext &cmd) const = 0;

			std::string getName() const {
				return this->name;
			}

			bool isNeededToBeAutorized() const {
				return this->needToBeAuthorized;
			}

			bool isNeededToBeOperator() const {
				return this->needToBeOperator;
			}
	};
}
