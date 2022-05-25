/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 17:53:17 by tglory            #+#    #+#             */
/*   Updated: 2022/05/25 18:31:14 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"
#include "../includes/ServerConfig.hpp"
#include "../includes/ServerIRC.hpp"


static ft::ServerIRC server;

void close_handler(int signal) {
	(void)signal;
	std::stringstream ss;
	ss << INFO << C_RED << "Try to force kill the server..." << C_RESET << std::endl;
	ft::logAndPrint(ss.str());
	server.setTryingToStop();
}

int main(const int ac, const char *av[]) {
	ft::ServerConfig servConfig;

	ft::log("------------------------------------\n");
	if (DEBUG_MODE) {
		std::stringstream ss;
		ss << DEBUG << "Welcome in debug mode." << C_RESET << std::endl;
		ft::logAndPrint(ss.str());
	}
	signal(SIGINT, close_handler);

	if (!servConfig.set(ac, av))
		return 1;
	servConfig.setIP("0.0.0.0");

	server.setConfig(servConfig);

	if (!server.start())
		return 1;

	server.execute();

	if (server.isEnabled())
		server.stop();
	return 0;
}
