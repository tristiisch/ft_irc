/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PartCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 21:32:35 by tglory            #+#    #+#             */
/*   Updated: 2022/05/18 20:36:14 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ClientCommand.hpp"

namespace ft {
	class PartCommand : public ClientCommand {

		public :
			PartCommand();

			~PartCommand();

			bool execute(CommandContext &cmd) const;
	};
}
