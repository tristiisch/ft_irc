/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NoticeCommand.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alganoun <alganoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 21:08:41 by alganoun          #+#    #+#             */
/*   Updated: 2022/05/12 21:09:43 by alganoun         ###   ########.fr       */
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
