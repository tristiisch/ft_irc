/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientCommand.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 18:53:37 by tglory            #+#    #+#             */
/*   Updated: 2022/05/18 20:30:28 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../ft_irc.hpp"
#include "../ClientIRC.hpp"
#include "../ServerIRC.hpp"
#include "CommandContext.hpp"
namespace ft {
	class ClientCommand {

		protected:
			std::string name;
			bool needToBeAuthorized;
			bool needToBeOperator;

		public :
			ClientCommand(std::string name);

			ClientCommand(std::string name, bool needToBeAuthorized, bool needToBeOperator);
			
			ClientCommand &operator=(ClientCommand const &instance);

			virtual ~ClientCommand() {};

			bool virtual execute(CommandContext &cmd) const = 0;

			std::string getName() const;

			bool isNeededToBeAutorized() const;

			bool isNeededToBeOperator() const;
	};
}
