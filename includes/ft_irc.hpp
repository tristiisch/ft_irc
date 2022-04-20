/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 17:43:20 by tglory            #+#    #+#             */
/*   Updated: 2022/04/20 21:55:20 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <cstdlib>
#include <cerrno>
#include <string>
#include <stdio.h>
#include <string.h>
#include <sstream>

#include <sys/socket.h> // socket, setsockopt, getsockname, bind, connect, listen, accept, send, recv
#include <sys/types.h> // getaddrinfo
#include <netdb.h> // getprotobyname, gethostbyname, getaddrinfo, freeaddrinfo
#include <arpa/inet.h> // htons, htonl, ntohs, ntohl, inet_addr, inet_ntoa
#include <signal.h> // signal
#include <unistd.h> // lseek
#include <sys/stat.h> // fstat
#include <fcntl.h> // fcntl
#include <poll.h> // poll

#include <netinet/in.h>

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)

#define C_RED "\033[0;31m"
#define C_YELLOW "\033[33m"
#define C_BLUE "\033[1;34m"
#define C_RESET "\033[0m"

#define ERROR C_RED << "ERROR > "
#define WARN C_YELLOW << "WARN > "

#ifndef DEBUG
# define DEBUG 0
#endif

namespace ft {

	template <class T>
	std::string toString(T obj) {
		std::stringstream ss;
		ss << obj;
		return ss.str();
	}

	template <class T>
	const char* toPChar(T obj) {
		return toString(obj).c_str();
	}

	template <class A>
	int checkError(const int ret, const char *msg, A *arg) {
		if (ret == -1) {
			std::cerr << C_RED;
			if (arg != NULL) {
				std::cerr << msg << " ";
				perror(toPChar(*arg));
			} else
				perror(msg);
			std::cerr << C_RESET;
			return errno;
		}
		return 0;
	}
}