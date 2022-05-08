/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 17:53:17 by tglory            #+#    #+#             */
/*   Updated: 2022/05/08 18:38:04 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"
#include "../includes/ServerConfig.hpp"
#include "../includes/ServerIRC.hpp"

ft::ServerIRC server;

void close_handler(int signal) {
	(void)signal;
	server.stop();
}

int main(const int ac, const char *av[]) {
	ft::ServerConfig servConfig;

	if (DEBUG) {
		std::cout << C_BLUE << "Welcome in debug mode." << C_RESET << std::endl;
	}
	signal(SIGINT, close_handler);

	if (!servConfig.set(ac, av))
		return 1;

	server.setConfig(servConfig);

	if (!server.start())
		return 1;

	// int i = 3;
	// while (i--) {
		server.task();
	// }

	server.stop();

	return 0;
}
