/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 17:43:20 by tglory            #+#    #+#             */
/*   Updated: 2022/04/19 19:32:05 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <cstdlib>
#include <cerrno>
#include <string>

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
