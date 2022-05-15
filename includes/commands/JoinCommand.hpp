/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 21:32:35 by tglory            #+#    #+#             */
/*   Updated: 2022/05/13 07:41:30 by tglory           ###   ########lyon.fr   */
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
	};
}
