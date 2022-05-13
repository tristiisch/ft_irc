/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 18:34:56 by tglory            #+#    #+#             */
/*   Updated: 2022/05/13 07:35:53 by tglory           ###   ########lyon.fr   */
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

		while ((pos = str.find(delimt)) != std::string::npos && str[0] != ':') // À vérifier
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
	

	void logAndPrint(std::string msg) {
		std::stringstream ss;
		std::string str;
		std::ofstream myfile;
		std::time_t t = std::time(0);   // get time now
		std::tm* now = std::localtime(&t);
		
		ss << '[' << now->tm_hour << ':' << now->tm_min << ':' << now->tm_sec << "] " << msg;
		str = ss.str();

		std::cout << str;
		myfile.open("log.txt", std::fstream::app);
		myfile << str;
		myfile.close();
	}

	void logAndPrint(std::ostream &out, std::string msg) {
		std::stringstream ss;
		std::string str;
		std::ofstream myfile;
		std::time_t t = std::time(0);   // get time now
		std::tm* now = std::localtime(&t);
		
		ss << '[' << now->tm_hour << ':' << now->tm_min << ':' << now->tm_sec << "] " << msg;
		str = ss.str();

		out << str;
		myfile.open("log.txt", std::fstream::app);
		myfile << str;
		myfile.close();
	}

	void log(std::string msg) {
		std::stringstream ss;
		std::string str;
		std::ofstream myfile;
		std::time_t t = std::time(0);   // get time now
		std::tm* now = std::localtime(&t);
		
		ss << '[' << now->tm_hour << ':' << now->tm_min << ':' << now->tm_sec << "] " << msg;
		str = ss.str();

		myfile.open("log.txt", std::fstream::app);
		myfile << str;
		myfile.close();
	}

	void logCommand(ClientIRC *client, std::string msg) {
		std::stringstream ss;
		std::string str;
		std::ofstream myfile;
		
		ss << *client << " > " << msg << std::endl;
		str = ss.str();

		myfile.open("commands.txt", std::fstream::app);
		myfile << str;
		myfile.close();
	}
}
