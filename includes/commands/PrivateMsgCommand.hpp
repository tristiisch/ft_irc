/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivateMsgCommand.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 19:24:32 by alganoun          #+#    #+#             */
/*   Updated: 2022/05/13 07:37:18 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ClientCommand.hpp"

namespace ft {
	class PrivateMsgCommand : public ClientCommand {

		public :
			PrivateMsgCommand();

			~PrivateMsgCommand();

			bool execute(CommandContext &cmd) const;
	};
}
