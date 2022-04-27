/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 17:53:17 by tglory            #+#    #+#             */
/*   Updated: 2022/04/27 18:13:18 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"
#include "../includes/ServerConfig.hpp"
#include "../includes/ServerIRC.hpp"


int main(const int ac, const char *av[]) {
	ft::ServerConfig servConfig;
	ft::ServerIRC server;

	if (DEBUG) {
		std::cout << C_BLUE << "Welcome in debug mode." << C_RESET << std::endl;
	}
	if (!servConfig.set(ac, av))
		return 1;

	server.setConfig(servConfig);

	if (!server.start())
		return 1;

	server.task();

	server.stop();

	return 0;
}
