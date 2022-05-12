/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alganoun <alganoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 17:43:20 by tglory            #+#    #+#             */
/*   Updated: 2022/05/12 21:07:20 by alganoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <cstdlib>
#include <cerrno>
#include <string>
#include <stdio.h>
#include <sstream>
#include <cstring>
#include <vector>
#include <map>
#include <algorithm>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#include <winsock2.h>

#elif defined (linux) || defined(_POSIX_VERSION) || __unix__  || __linux__ || defined(_POSIX_VERSION) || __APPLE__

# include <sys/socket.h> // socket, setsockopt, getsockname, bind, connect, listen, accept, send, recv
# include <sys/types.h> // getaddrinfo
# include <netdb.h> // getprotobyname, gethostbyname, getaddrinfo, freeaddrinfo
# include <arpa/inet.h> // htons, htonl, ntohs, ntohl, inet_addr, inet_ntoa
# include <signal.h> // signal
# include <unistd.h> // lseek
# include <sys/stat.h> // fstat
# include <fcntl.h> // fcntl
# include <poll.h> // poll
#include <sys/types.h> // recv
#include <sys/socket.h> // recv

# include <netinet/in.h>

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

# define INVALID_SOCKET -1
# define SOCKET_ERROR -1
# define closesocket(s) close(s)

#else
# error "Unknown compiler"
#endif

#define C_RED "\033[31m"
#define C_GREEN "\033[32m"
#define C_YELLOW "\033[33m"
#define C_BLUE "\033[1;34m"
#define C_RESET "\033[0m"

#define ERROR C_RED << "ERROR > "
#define WARN C_YELLOW << "WARN > "


#define MSG_DELIMITER "\r\n"

//il faut encore faire des define sur les messages d'erreur.
#define ERR_NOSUCHCHANNEL(channel) channel + " :no such channel"
#define ERR_NOSUCHNICK(nick) nick + " :No such nick/channel"


#ifndef DEBUG
# define DEBUG 0
#endif

namespace ft {

	std::string join(std::vector<std::string> &vector, std::string delim);
	void split(std::string str, std::string delimt, void (*f)(std::string));
	std::vector<std::string> split(std::string str, std::string delimt);
	
	std::ostream &operator<<(std::ostream &outputFile, const SOCKADDR_IN &csin);

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

	template <class T>
	int checkError(const int ret, const char *msg, T *arg) {
		if (ret <= -1) {
			std::cerr << C_RED;
			if (arg != NULL)
				std::cerr << msg << " " << toString(*arg) << ": " << std::strerror(errno) << std::endl;
			else
				std::cerr << msg << ": " << std::strerror(errno) << std::endl;
			std::cerr << C_RESET;
			return errno;
		}
		return 0;
	}
}
