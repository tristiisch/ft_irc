/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerIRC.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 21:19:26 by tglory            #+#    #+#             */
/*   Updated: 2022/04/20 21:46:24 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include "ft_irc.hpp"
#include "ServerConfig.hpp"

namespace ft {
	class ServerIRC {

		private:
			bool enabled;
			ServerConfig config;
			SOCKET sock;

		public :
			ServerIRC() : enabled(false) {}

			ServerIRC(ServerConfig& config) : enabled(false), config(config) {}

			ServerIRC& operator=(const ServerIRC& x)
			{
				this->setConfig(x.getConfig());
				return *this;
			}

			~ServerIRC() {}

			const ServerConfig& getConfig() const;

			bool isEnabled() const;

			bool setConfig(const ServerConfig& config);

			bool start();

			void task();

			bool stop();
	};
}
