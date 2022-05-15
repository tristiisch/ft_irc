/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivateMsgCommand.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alganoun <alganoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 19:24:32 by alganoun          #+#    #+#             */
/*   Updated: 2022/05/12 19:25:28 by alganoun         ###   ########.fr       */
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
