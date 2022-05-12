/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 18:34:56 by tglory            #+#    #+#             */
/*   Updated: 2022/05/09 02:06:48 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"
#include "../includes/ClientIRC.hpp"

namespace ft {

	std::ostream &operator<<(std::ostream &outputFile, const ClientIRC &client) {
		if (!client.getNick().empty()) {
			outputFile << client.getNick();
		} else 
			outputFile << inet_ntoa(client.getAddress()) << ":" << client.getPort();
		outputFile << "#" << client.getSocket();
		return outputFile;
	}

	std::ostream &operator<<(std::ostream &outputFile, const SOCKADDR_IN &csin) {
		outputFile << inet_ntoa(csin.sin_addr) << ":" << csin.sin_port;
		return outputFile;
	}

	void split(std::string str, std::string delimt, void (*f)(std::string)) {
		std::string tmp;
		size_t pos = 0;

		while ((pos = str.find(delimt)) != std::string::npos)
		{
			tmp = str.substr(0, pos - 1);
			f(tmp);
			str.erase(0, pos + delimt.length());
		}
		if (!str.empty())
			f(str);
	}

	std::vector<std::string> split(std::string str, std::string delimt) {
		std::vector<std::string> vector;
		std::string tmp;
		size_t pos = 0;

		while ((pos = str.find(delimt)) != std::string::npos)
		{
			tmp = str.substr(0, pos - 1);
			vector.push_back(tmp);
			str.erase(0, pos + delimt.length());
		}
		if (!str.empty())
			vector.push_back(str);
		return vector;
	}

	std::string join(std::vector<std::string> &vector, std::string delim) {
		std::stringstream ss;
		for (std::vector<std::string>::iterator it = vector.begin(); it != vector.end(); ++it) {
			ss << *it;
			if (it + 1 != vector.end()) {
				ss << delim;
			}
		}
		return ss.str();
	}
}
