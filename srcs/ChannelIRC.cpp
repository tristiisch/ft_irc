/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelIRC.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alganoun <alganoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/07 19:45:36 by alganoun          #+#    #+#             */
/*   Updated: 2022/05/07 21:06:58 by alganoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../includes/ChannelIRC.hpp"

namespace ft
{
	ChannelIRC::ChannelIRC() {}

	ChannelIRC::ChannelIRC(const char *name)
	:	client_list(),
		name(name)
	{}

	ChannelIRC::~ChannelIRC() {}

	void	ChannelIRC::setName(const char *name)
	{ 
		this->name = name;
	}

	void	ChannelIRC::addUser(ClientIRC *const &to_add)
	{
		//il faut verifier sur le client existe déjà
		this->client_list.push_back(to_add);
	}
}