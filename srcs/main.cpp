/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 17:53:17 by tglory            #+#    #+#             */
/*   Updated: 2022/04/20 02:02:18 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"
#include "../includes/ServerConfig.hpp"

namespace ft {

	int checkUsage(const int ac, const char *av[], ServerConfig& servConfig) {
		if (ac >= 3) {
			if (!servConfig.setPort(av[1])) {
				std::cerr << "\033[0;31mError > Port should be between 0-65535\033[0m" << std::endl;
				return 1;
			} else if (!servConfig.setPassword(av[2])) {
				std::cerr << "\033[0;31mError > Password not valid\033[0m" << std::endl;
				return 1;
			}
			return 0;
		}
		std::cerr << "\033[0;31mUsage > " << av[0] << " <port> <password>\033[0m" << std::endl;
		return 1;
	}

	int setup(ServerConfig& servConfig) {
		SOCKADDR_IN sin, csin;
    	SOCKET sock, csock;
		int ret;

		sin.sin_addr.s_addr = inet_addr("127.0.0.1");
		sin.sin_family = PF_INET;
		sin.sin_port = htons(servConfig.getPort());

		sock = socket(PF_INET, SOCK_STREAM, 0);
		if (ft::checkError(sock, "Error with socket creation", (char*) NULL))
			return errno;
		ret = bind(sock, (SOCKADDR *)&sin, sizeof(sin));
		if (ft::checkError(ret, "Error while binding port", &servConfig.getPort()))
			return errno;
		ret = listen(sock, 0);
		if (ft::checkError(ret, "Error while listen port", &servConfig.getPort()))
			return errno;
		while(1) /* Boucle infinie. Exercice : améliorez ce code. */
		{
			socklen_t sinsize = sizeof(csin);
			if ((csock = accept(sock, (SOCKADDR *)&csin, &sinsize)) != INVALID_SOCKET) {
				send(csock, "Hello world!\r\n", 14, 0);
				break;
			}
		}
		closesocket(sock);
		return 0;
	}
}


int main(const int ac, const char *av[]) {
	ft::ServerConfig servConfig;
	int setupRet;

	if (ft::checkUsage(ac, av, servConfig))
		return 1;

	setupRet = ft::setup(servConfig);
	if (setupRet)
		return setupRet;
	return 0;
}
