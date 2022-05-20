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
#include "ClientIRC.hpp"

namespace ft {

	class ChannelIRC {

		private:
		std::string _name;
		std::vector<ClientIRC *> _client_list;
		std::vector<ClientIRC *> _ban_list;
		std::vector<ClientIRC *> _ope_list;
		int _size;
		int _max_size;

		public :
			ChannelIRC();
			ChannelIRC(const char *name);
			ChannelIRC(const char *name, int const &max_size);
			~ChannelIRC();

			const std::string& getName() const;
			int			getSize();
			int			getMaxSize();
			std::vector<ClientIRC *>& getClientList();
			std::vector<ClientIRC *>& getBanList();
			std::vector<ClientIRC *>& getOpeList();

			void	setName(const char *name);

			int		addUser(ClientIRC *const &to_add);
			int		addOperator(ClientIRC *const &to_add);
			int		addBannedUser(ClientIRC *const &to_add);
			int		removeUser(ClientIRC *const &to_remove);
			int		removeOperator(ClientIRC *const &to_remove);
			int		unbanUser(ClientIRC *const &to_unban);

			void	sendMessageToAll(ClientIRC *const &sender, std::string const &message);

	};

	std::vector<ClientIRC *>::iterator ClientIterator
		(ClientIRC *const &to_find, std::vector<ClientIRC *> &to_check);

	bool			clientExists(ClientIRC *const &to_add, std::vector<ClientIRC *> &to_check);
}
