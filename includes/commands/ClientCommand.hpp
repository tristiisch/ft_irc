/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientCommand.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 18:53:37 by tglory            #+#    #+#             */
/*   Updated: 2022/05/08 19:36:17 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include "../ft_irc.hpp"
#include "../ClientIRC.hpp"
#include "../ServerIRC.hpp"

namespace ft {
	class ClientCommand {

		protected:
			std::string name;

		public :
			ClientCommand(std::string name) : name(name) {}
			ClientCommand &operator=(ClientCommand const &instance);

			virtual ~ClientCommand() {};

			void virtual execute(ServerIRC &server, ClientIRC *client, std::string &cmd, std::string &args) const = 0;
			
			std::string getName() const {
				return this->name;
			}
	};
}
