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

	struct cl_info
	{

		ClientIRC *user;
		int banned;
		int ope;

		cl_info();
		cl_info(ClientIRC *const &to_add);
		~cl_info();
	};

	class ChannelIRC {

		private:
		std::string _name;
		std::vector<cl_info> _client_list;
		int _size;
		int _max_size;

		public :
			ChannelIRC();
			ChannelIRC(const char *name, ClientIRC *const &first_client);
			ChannelIRC(const char *name, ClientIRC *const &first_client, int const &max_size);
			~ChannelIRC();

			std::string getName();
			int			getSize();
			int			getMaxSize();
			std::vector<cl_info> getClientList();

			void	setName(const char *name);

			void	addUser(ClientIRC *const &to_add);
			void	addOperator(ClientIRC *const &to_add);
			void	addBannedUser(ClientIRC *const &to_add);
			void	removeUser(ClientIRC *const &to_remove);
			void	removeOperator(ClientIRC *const &to_remove);
			void	unbanUser(ClientIRC *const &to_unban);

			void	sendMessageToAll(ClientIRC *const &sender, std::string const &message);

	};

	bool							clientExists(ClientIRC *const &to_add, std::vector<cl_info> &to_check);
	std::vector<cl_info>::iterator 	clientSearch (std::vector<cl_info> cl_list, ClientIRC *const &to_search);
}
