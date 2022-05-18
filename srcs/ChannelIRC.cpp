/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelIRC.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/07 19:45:36 by alganoun          #+#    #+#             */
/*   Updated: 2022/05/18 16:22:28 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ChannelIRC.hpp"

namespace ft
{
	//Classe de channel
	ChannelIRC::ChannelIRC() {}

	ChannelIRC::ChannelIRC(const char *name, ClientIRC *const &first_client)
	:	_name(name),
		_client_list(),
		_ban_list(),
		_ope_list(),
		_size(1),
		_max_size(42)
	{
		std::cout << "Channel \"" << this->_name << "\" created." << std::endl;
		this->_client_list.push_back(first_client);
		std::cout << "The " << this->_name << " channel was created by the client n. " << first_client->getId() << std::endl;
	}

	ChannelIRC::ChannelIRC(const char *name, ClientIRC *const &first_client, int const &max_size)
	:	_name(name),
		_client_list(),
		_ban_list(),
		_ope_list(),
		_size(1),
		_max_size(max_size)
	{
		this->_client_list.push_back(first_client);
		std::cout << "The " << this->_name << " channel of size "
						<< this->_max_size << " was created by the client n. " << first_client->getId() << std::endl;
	}

	ChannelIRC::~ChannelIRC() {}

	const std::string	ChannelIRC::getName() const
	{
		return (this->_name);
	}

	std::vector<ClientIRC *> ChannelIRC::getClientList()
	{
		return this->_client_list;
	}

	std::vector<ClientIRC *> ChannelIRC::getBanList()
	{
		return this->_ban_list;
	}

	std::vector<ClientIRC *> ChannelIRC::getOpeList()
	{
		return this->_ope_list;
	}

	void	ChannelIRC::setName(const char *name)
	{
		this->_name = name;
	}

	int		ChannelIRC::addUser(ClientIRC *const &to_add)
	{
		if (clientExists(to_add, this->_client_list) == true)
		{
			std::cout << "The User " << to_add->getNick() << " is already added to the channel." << std::endl;
			return (ALREADY_IN_CHANNEL);
		}
		this->_client_list.push_back(to_add);
		if (_size < _max_size)
			this->_size++;
		else
		{
			std::cout << "The User " << to_add->getNick() << " cannot be added to a full channel." << std::endl;
			return (CHANNEL_FULL);
		}
		return true;
	}

	int		ChannelIRC::addOperator(ClientIRC *const &to_add)
	{
		if (clientExists(to_add, this->_ope_list))
		{
			std::cout << "The User " << to_add->getNick() << " is already operator in this channel." << std::endl;
			return ALREADY_OPERATOR;
		}
		this->_ope_list.push_back(to_add);
		return true;
	}

	int		ChannelIRC::addBannedUser(ClientIRC *const &to_add)
	{
		if (clientExists(to_add, this->_ban_list))
		{
			std::cout << "The User " << to_add->getNick() << " is already banned from the channel." << std::endl;
			return ALREADY_BANNED;
		}
		this->_ban_list.push_back(to_add);
		if (clientExists(to_add, this->_client_list))
		{
			removeUser(to_add);
			this->_size--;
		}
		return true;
	}

	int		ChannelIRC::removeUser(ClientIRC *const &to_remove)
	{
		if (clientExists(to_remove, this->_client_list) == false)
		{
			std::cout << "The User " << to_remove->getNick() << " does not exist in this channel." << std::endl;
			return NO_SUCH_NICK;
		}
		std::vector<ClientIRC *>::iterator ite = ClientIterator(to_remove, _client_list);
		this->_client_list.erase(ite);
		this->_size--;
		return true;
	}

	int		ChannelIRC::removeOperator(ClientIRC *const &to_remove)
	{
		if (clientExists(to_remove, this->_ope_list) == false)
		{
			std::cout << "The User " << to_remove->getNick() << " is not an operator in this channel." << std::endl;
			return NO_SUCH_NICK;
		}
		std::vector<ClientIRC *>::iterator ite = ClientIterator(to_remove, _ope_list);
		this->_ope_list.erase(ite);
		return true;
	}

	int		ChannelIRC::unbanUser(ClientIRC *const &to_unban)
	{
		if (clientExists(to_unban, this->_ban_list) == false)
		{
			std::cout << "The User " << to_unban->getNick() << " is not banned from the channel." << std::endl;
			return NO_SUCH_NICK;;
		}
		std::vector<ClientIRC *>::iterator ite = ClientIterator(to_unban, _ban_list);
		this->_ban_list.erase(ite);
		return true;
	}

	void	ChannelIRC::sendMessageToAll(ClientIRC *const &sender, std::string const &message)
	{
		std::vector<ClientIRC *>::iterator ite = _client_list.begin();
		std::string channel_msg = message + sender->getDelimiter();

		while(ite != _client_list.end())
		{
			if ((*ite)->getId() != sender->getId())
				sender->sendMessage(*ite, channel_msg.c_str());
			ite++;
		}
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::vector<ClientIRC *>::iterator ClientIterator
		(ClientIRC *const &to_find, std::vector<ClientIRC *> &to_check)
	{
		std::vector<ClientIRC *>::iterator ite = to_check.begin();
		while ((*ite)->getId() != to_find->getId())
			ite++;
		return ite;
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
