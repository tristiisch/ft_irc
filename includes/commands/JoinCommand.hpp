/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alganoun <alganoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 21:32:35 by tglory            #+#    #+#             */
/*   Updated: 2022/05/18 17:59:02 by alganoun         ###   ########.fr       */
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
			void ErrorManagement(int ret, ClientIRC *const &client, std::string const &arg) const;
	};
}
