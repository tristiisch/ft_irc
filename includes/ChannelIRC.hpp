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

	struct cl_info
	{

		ClientIRC *user;
		int banned;
		int ope;

		cl_info();
		cl_info(ClientIRC *to_add);
		~cl_info();
	};

	class ChannelIRC {

		private:
		std::string name;
		std::vector<cl_info> client_list;
		int size;
		int max_size;

		public :
			ChannelIRC();
			ChannelIRC(const char *name);
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



	};
}
