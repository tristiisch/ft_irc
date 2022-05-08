/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 18:34:56 by tglory            #+#    #+#             */
/*   Updated: 2022/05/08 18:38:35 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

namespace ft {

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
}