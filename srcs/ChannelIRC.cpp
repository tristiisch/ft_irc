/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelIRC.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allanganoun <allanganoun@student.42lyon    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/07 19:45:36 by alganoun          #+#    #+#             */
/*   Updated: 2022/05/15 14:11:22 by allanganoun      ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ChannelIRC.hpp"

namespace ft
{

	//Structure de stockage des infos des clients
	cl_info::cl_info()
	:	user(NULL),
		banned(0),
		ope(0)
	{}

	cl_info::cl_info(ClientIRC *const &to_add)
	:	user(to_add),
		banned(0),
		ope(0)
	{}

	cl_info::~cl_info() {}


	//Classe de channel
	ChannelIRC::ChannelIRC() {}

	ChannelIRC::ChannelIRC(const char *name, ClientIRC *const &first_client)
	:	_name(name),
		_client_list(),
		_size(1),
		_max_size(42)
	{
		std::cout << "Channel \"" << this->_name << "\" created." << std::endl;
		this->_client_list.push_back(first_client);
		std::cout << "The " << this->_name << "channel was created by the client n. " << first_client->getId() << std::endl;
	}

	ChannelIRC::ChannelIRC(const char *name, ClientIRC *const &first_client, int const &max_size)
	:	_name(name),
		_client_list(),
		_size(1),
		_max_size(max_size)
	{
		this->_client_list.push_back(first_client);
		std::cout << "The " << this->_name << "channel of size "
						<< this->_max_size << " was created by the client n. " << first_client->getId() << std::endl;
	}

	ChannelIRC::~ChannelIRC() {}

	std::string	ChannelIRC::getName()
	{
		return (this->_name);
	}

	std::vector<cl_info> ChannelIRC::getClientList()
	{
		return this->_client_list;
	}

	void	ChannelIRC::setName(const char *name)
	{
		this->_name = name;
	}

	void	ChannelIRC::addUser(ClientIRC *const &to_add)
	{
		if (clientExists(to_add, this->_client_list) == true)
		{
			std::cout << "The User " << to_add->getNick() << "is already added to the channel." << std::endl;
			//envoyer l'info au client avec le bon code
			return;
		}
		cl_info nclient(to_add);
		this->_client_list.push_back(nclient);
		if (_size < _max_size)
			this->_size++;
		else
		{
			std::cout << "The User " << to_add->getNick() << "cannot be added to a full channel." << std::endl;
			// send au client ce qu' il faut
		}

	}

	void	ChannelIRC::addOperator(ClientIRC *const &to_add)
	{
		std::vector<cl_info>::iterator ite  = clientSearch(this->_client_list, to_add);
		if (ite == this->_client_list.end() || ite->ope == 1)
		{
			std::cout << "The User " << to_add->getNick() << "is already operator in this channel." << std::endl;
			//envoyer l'info au client avec le bon code
			return;
		}
		ite->ope= 1;
	}

	void	ChannelIRC::addBannedUser(ClientIRC *const &to_add)
	{
		std::vector<cl_info>::iterator ite  = clientSearch(this->_client_list, to_add);
		if (ite == this->_client_list.end() || ite->banned == 1)
		{
			std::cout << "The User " << to_add->getNick() << "is already banned from the channel." << std::endl;
			//envoyer l'info au client avec le bon code
			return;
		}
		ite->banned = 1;
		this->_size--;
	}

	void	ChannelIRC::removeUser(ClientIRC *const &to_remove)
	{
		if (clientExists(to_remove, this->_client_list) == false)
		{
			std::cout << "The User " << to_remove->getNick() << "does not exist in this channel." << std::endl;
			//envoyer l'info au client avec le bon code
			return;
		}
		std::vector<cl_info>::iterator ite = _client_list.begin();
		while (ite->user->getId() != to_remove->getId())
			ite++;
		this->_client_list.erase(ite);
		this->_size--;
	}

	void	ChannelIRC::removeOperator(ClientIRC *const &to_remove)
	{
		std::vector<cl_info>::iterator ite  = clientSearch(this->_client_list, to_remove);
		if (ite == this->_client_list.end() || ite->banned == 0)
		{
			std::cout << "The User " << to_remove->getNick() << "is not an operator in this channel." << std::endl;
			//envoyer l'info au client avec le bon code
			return;
		}
		ite->ope = 0;
	}

	void	ChannelIRC::unbanUser(ClientIRC *const &to_unban)
	{
		std::vector<cl_info>::iterator ite  = clientSearch(this->_client_list, to_unban);
		if (ite == this->_client_list.end() || ite->banned == 0)
		{
			std::cout << "The User " << to_unban->getNick() << "is not banned from the channel." << std::endl;
			//envoyer l'info au client avec le bon code
			return;
		}
		ite->banned = 0;
	}

	void	ChannelIRC::sendMessageToAll(ClientIRC *const &sender, std::string const &message)
	{
		std::vector<cl_info>::iterator ite = _client_list.begin();
		std::string channel_msg = message + sender->getDelimiter();

		while(ite != _client_list.end())
		{
			if (ite->banned == 0 && ite->user != sender)
				sender->sendMessage(ite->user, channel_msg.c_str());
			ite++;
		}
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	std::vector<cl_info>::iterator clientSearch
	(std::vector<cl_info> cl_list, ClientIRC *const &to_search)
	{
		std::vector<cl_info>::iterator ite = cl_list.begin();
		while (ite->user->getId() != to_search->getId())
			ite++;
		return ite;
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
