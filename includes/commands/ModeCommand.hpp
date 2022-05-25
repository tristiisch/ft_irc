/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/22 20:33:03 by alganoun          #+#    #+#             */
/*   Updated: 2022/05/25 18:42:02 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ClientCommand.hpp"

namespace ft {
	class ModeCommand : public ClientCommand {

		public :
			ModeCommand();

			~ModeCommand();

			bool execute(CommandContext &cmd) const;
			void	errorManagement(int ret, ClientIRC *const &client, std::string const &arg) const;
	};
}