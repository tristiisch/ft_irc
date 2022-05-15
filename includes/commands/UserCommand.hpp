/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allanganoun <allanganoun@student.42lyon    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 16:44:18 by allanganoun       #+#    #+#             */
/*   Updated: 2022/05/15 16:46:07 by allanganoun      ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ClientCommand.hpp"
#include "../ChannelIRC.hpp"

namespace ft {
	class UserCommand : public ClientCommand {

		public :
			UserCommand();

			~UserCommand();

			bool execute(CommandContext &cmd) const;
	};
}
