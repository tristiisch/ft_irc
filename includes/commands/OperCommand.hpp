/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OperCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 18:53:37 by tglory            #+#    #+#             */
/*   Updated: 2022/05/23 15:39:22 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ClientCommand.hpp"

namespace ft {
	class OperCommand : public ClientCommand {

		public :
			OperCommand();

			~OperCommand();

			bool execute(CommandContext &cmd) const;
	};
}
