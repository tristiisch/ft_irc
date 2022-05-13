/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NoticeCommand.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 21:08:41 by alganoun          #+#    #+#             */
/*   Updated: 2022/05/13 07:37:31 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ClientCommand.hpp"

namespace ft {
	class NoticeCommand : public ClientCommand {

		public :
			NoticeCommand();

			~NoticeCommand();

			bool execute(CommandContext &cmd) const;
	};
}
