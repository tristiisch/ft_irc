/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelIRC.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 16:32:12 by tglory            #+#    #+#             */
/*   Updated: 2022/05/04 16:55:55 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.hpp"
#include "ServerIRC.hpp"

namespace ft {
	class ChannelIRC {

		private:
		std::string name;
		std::vector<ClientIRC *> client_list;
		std::vector<ClientIRC *> ban_list;
		int size;

		public :
			ChannelIRC();
			ChannelIRC(const char *name);
			~ChannelIRC();

			std::string getName();
			int			getSize();
			std::vector<ClientIRC * > getClientList();
			std::vector<ClientIRC * > getBanList();

			void	setName(const char *name);

			void	addUser(ClientIRC *const &to_add);
			void	addBannedUser(ClientIRC *const &to_add);
			void	removeUser(ClientIRC *const &to_remove);
			void	unbanUser(ClientIRC *const &to_unban);



	};
}
