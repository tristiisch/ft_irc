/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CapCommand.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 18:53:37 by tglory            #+#    #+#             */
/*   Updated: 2022/05/08 20:16:57 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../ft_irc.hpp"
#include "../ClientIRC.hpp"
#include "../ServerIRC.hpp"
#include "ClientCommand.hpp"

namespace ft {
	class CapCommand : public ClientCommand {

		public :
			CapCommand();

			~CapCommand();

			void execute(ServerIRC &server, ClientIRC *client, std::string &cmd, std::string &args) const;
	};
}
