/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alganoun <alganoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 19:06:54 by alganoun          #+#    #+#             */
/*   Updated: 2022/05/12 19:07:11 by alganoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ClientCommand.hpp"

namespace ft {
	class KickCommand : public ClientCommand {

		public :
			KickCommand();

			~KickCommand();

			bool execute(CommandContext &cmd) const;
	};
}
