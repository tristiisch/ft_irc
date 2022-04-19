/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 17:53:17 by tglory            #+#    #+#             */
/*   Updated: 2022/04/19 19:38:35 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"
#include "../includes/ServerConfig.hpp"

namespace ft {
	int checkUsage(const int ac, const char *av[], ServerConfig& servConfig) {
		if (ac >= 3 && servConfig.setPort(atoi(av[1])) && servConfig.setPassword(av[2]))
			return 0;
		std::cerr << "\033[0;31mUsage > " << av[0] << " <port> <password>\033[0m" << std::endl;
		return 1;
	}

	int checkError(const int ret, const char *msg, const char *arg) {
		if (ret == -1) {
			std::cerr << "\033[0;31m";
			if (arg != NULL) {
				std::cerr << msg << " ";
				perror(arg);
			} else
				perror(msg);
			std::cerr << "\033[0m";
			return errno;
		}
		return 0;
	}

	std::string toString(const bool b) {
		std::stringstream ss;
		ss << b;
		return ss.str();
	}

	std::string toString(const int i) {
		std::stringstream ss;
		ss << i;
		return ss.str();
	}
}

int main(const int ac, const char *av[], const char **envp) {
	SOCKADDR_IN sin;
	ServerConfig servConfig;
	int sock;

	(void)envp;
	if (ft::checkUsage(ac, av, servConfig))
		return 1;

	sin.sin_addr.s_addr = inet_addr("127.0.0.1");
	sin.sin_family = AF_INET;
	sin.sin_port = htons(servConfig.getPort());

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (ft::checkError(sock, "Error with socket creation", NULL))
		return errno;
	if (ft::checkError(bind(sock, (SOCKADDR *)&sin, sizeof(sin)), "Error while binding port", ft::toString(servConfig.getPort()).c_str()))
		return errno;
	closesocket(sock);
	return 0;
}
