/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelIRC.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allanganoun <allanganoun@student.42lyon    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/07 19:45:36 by alganoun          #+#    #+#             */
/*   Updated: 2022/05/09 21:05:59 by allanganoun      ###   ########lyon.fr   */
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

	int		ChannelIRC::getSize()
	{
		return(this->size);
	}

	std::string	ChannelIRC::getName()
	{
		return (this->name);
	}

	std::vector<ClientIRC *> ChannelIRC::getClientList()
	{
		return this->client_list;
	}

	std::vector<ClientIRC *> ChannelIRC::getBanList()
	{
		return this->ban_list;
	}

	int		ChannelIRC::getSize()
	{
		return this->size;
	}

	void	ChannelIRC::setName(const char *name)
	{
		this->name = name;
	}

	void	ChannelIRC::addUser(ClientIRC *const &to_add)
	{
		if (clientExists(to_add, this->client_list) == true)
		{
			std::cout << "The User " << to_add->getNick() << "is already added to the channel." << std::endl;
			//envoyer l'info au client avec le bon code
			return;
		}
		this->client_list.push_back(to_add);
		this->size++;
	}

	void	ChannelIRC::addBannedUser(ClientIRC *const &to_add)
	{
		if (clientExists(to_add, this->ban_list) == true)
		{
			std::cout << "The User " << to_add->getNick() << "is already banned from the channel." << std::endl;
			//envoyer l'info au client avec le bon code
			return;
		}
		this->ban_list.push_back(to_add);
	}

	void	ChannelIRC::removeUser(ClientIRC *const &to_remove)
	{
		if (clientExists(to_remove, this->client_list) == false)
		{
			std::cout << "The User " << to_remove->getNick() << "does not exist in this channel." << std::endl;
			//envoyer l'info au client avec le bon code
			return;
		}
		std::vector<ClientIRC *>::iterator ite = client_list.begin();
		while ((*ite)->getId() != to_remove->getId())
			ite++;
		this->client_list.erase(ite);
	}

	void	ChannelIRC::unbanUser(ClientIRC *const &to_unban)
	{
		if (clientExists(to_unban, this->ban_list) == false)
		{
			std::cout << "The User " << to_unban->getNick() << "is not banned in this channel." << std::endl;
			//envoyer l'info au client avec le bon code
			return;
		}
		std::vector<ClientIRC *>::iterator ite = ban_list.begin();
		while ((*ite)->getId() != to_unban->getId())
			ite++;
		this->ban_list.erase(ite);
	}

	bool	clientExists(ClientIRC *const &to_add, std::vector<ClientIRC *> &to_check)
	{
		std::vector<ClientIRC *>::iterator ite = to_check.begin();
		while ((*ite)->getId() != to_add->getId())
			ite++;
		if (ite == to_check.end())
			return (false);
		return (true);
	}
}
