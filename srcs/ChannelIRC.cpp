/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelIRC.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allanganoun <allanganoun@student.42lyon    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/07 19:45:36 by alganoun          #+#    #+#             */
/*   Updated: 2022/05/11 12:22:00 by allanganoun      ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../includes/ChannelIRC.hpp"

namespace ft
{

	//Structure de stockage des infos des clients
	cl_info::cl_info()
	:	user(NULL),
		banned(0),
		ope(0)
	{}

	cl_info::cl_info(ClientIRC *to_add)
	:	user(to_add),
		banned(0),
		ope(0)
	{}


	//Classe de channel
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

	std::vector<cl_info> ChannelIRC::getClientList()
	{
		return this->client_list;
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
		cl_info nclient(to_add);
		this->client_list.push_back(nclient);
		if (size < max_size)
			this->size++;
		else
		{
			std::cout << "The User " << to_add->getNick() << "cannot be added to a full channel." << std::endl;
			// send au client ce qu' il faut
		}

	}

	void	ChannelIRC::addBannedUser(ClientIRC *const &to_add)
	{
		std::vector<cl_info>::iterator ite  = clientSearch(this->client_list, to_add);
		if (ite == this->client_list.end() || ite->banned == 1)
		{
			std::cout << "The User " << to_add->getNick() << "is already banned from the channel." << std::endl;
			//envoyer l'info au client avec le bon code
			return;
		}
		ite->banned = 1;
	}

	void	ChannelIRC::removeUser(ClientIRC *const &to_remove)
	{
		if (clientExists(to_remove, this->client_list) == false)
		{
			std::cout << "The User " << to_remove->getNick() << "does not exist in this channel." << std::endl;
			//envoyer l'info au client avec le bon code
			return;
		}
		std::vector<cl_info>::iterator ite = client_list.begin();
		while (ite->user->getId() != to_remove->getId())
			ite++;
		this->client_list.erase(ite);
		this->size--;
	}

	void	ChannelIRC::unbanUser(ClientIRC *const &to_unban)
	{
		std::vector<cl_info>::iterator ite  = clientSearch(this->client_list, to_unban);
		if (ite == this->client_list.end() || ite->banned == 0)
		{
			std::cout << "The User " << to_unban->getNick() << "is not banned from the channel." << std::endl;
			//envoyer l'info au client avec le bon code
			return;
		}
		ite->banned = 0;
	}

	std::vector<cl_info>::iterator clientSearch
	(std::vector<cl_info> cl_list, ClientIRC *const &to_search)
	{
		std::vector<cl_info>::iterator ite = cl_list.begin();
		while (ite->user->getId() != to_search->getId())
			ite++;
	}

	bool	clientExists(ClientIRC *const &to_add, std::vector<cl_info> &to_check)
	{
		std::vector<cl_info>::iterator ite = to_check.begin();
		while (ite->user->getId() != to_add->getId())
			ite++;
		if (ite == to_check.end())
			return (false);
		return (true);
	}
}
