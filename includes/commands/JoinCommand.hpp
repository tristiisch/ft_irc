/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 21:32:35 by tglory            #+#    #+#             */
/*   Updated: 2022/05/25 18:42:02 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ClientCommand.hpp"
#include "../ChannelIRC.hpp"

namespace ft {
	class JoinCommand : public ClientCommand {

		public :
			JoinCommand();

			~JoinCommand();

			bool execute(CommandContext &cmd) const;
			void errorManagement(int ret, ClientIRC *const &client, std::string const &arg) const;
	};
}
