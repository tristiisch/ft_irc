/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 17:43:20 by tglory            #+#    #+#             */
/*   Updated: 2022/04/20 01:54:32 by tglory           ###   ########lyon.fr   */
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

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

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
			std::cerr << "\033[0;31m";
			if (arg != NULL) {
				std::cerr << msg << " ";
				perror(toPChar(*arg));
			} else
				perror(msg);
			std::cerr << "\033[0m";
			return errno;
		}
		return 0;
	}
}