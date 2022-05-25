/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelIRC.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/07 19:45:36 by alganoun          #+#    #+#             */
/*   Updated: 2022/05/25 18:41:42 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ChannelIRC.hpp"

namespace ft
{
	//Classe de channel
	ChannelIRC::ChannelIRC() {}

	ChannelIRC::ChannelIRC(const char *name)
	:	_name(name),
		_client_list(),
		_ban_list(),
		_ope_list(),
		_size(1),
		_max_size(42)
	{
		std::cout << "Channel \"" << this->_name << "\" created." << std::endl;
	}

	ChannelIRC::ChannelIRC(const char *name, ClientIRC *const &first_client)
	:	_name(name),
		_client_list(),
		_ban_list(),
		_ope_list(),
		_size(1),
		_max_size(42)
	{
		std::cout << "Channel \"" << this->_name << "\" created." << std::endl;
		this->addUser(first_client);
		this->addOperator(first_client);
		std::cout << "The " << this->_name << "channel was created by the client n. " << first_client->getId() << std::endl;
	}

	
	ChannelIRC::ChannelIRC(const char *name, int const &max_size)
	:	_name(name),
		_client_list(),
		_ban_list(),
		_ope_list(),
		_size(1),
		_max_size(max_size)
	{
		std::cout << "The " << this->_name << " channel of size "
						<< this->_max_size << " was created." << std::endl;
	}

	ChannelIRC::~ChannelIRC() {}

	const std::string&	ChannelIRC::getName() const
	{
		return (this->_name);
	}

	std::vector<ClientIRC *>& ChannelIRC::getClientList()
	{
		return this->_client_list;
	}

	std::vector<ClientIRC *>& ChannelIRC::getBanList()
	{
		return this->_ban_list;
	}

	std::vector<ClientIRC *>& ChannelIRC::getOpeList()
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
			return ALREADY_IN_CHANNEL;
		}
		else if (clientExists(to_add, this->_ban_list))
		{
			std::cout << "The User " << to_add->getNick() << " is banned from the channel." << std::endl;
			return USER_BANNED;
		}
		if ((int) _client_list.size() + 1 > _max_size) {
			std::cout << "The User " << to_add->getNick() << " cannot be added to a full channel." << std::endl;
			return CHANNEL_FULL;
		}
		this->_client_list.push_back(to_add);
		this->_size++;
		return true;
	}

	int		ChannelIRC::addOperator(ClientIRC *const &to_add)
	{
		if (!clientExists(to_add, this->_client_list))
			return NO_SUCH_NICK;
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

	void	ChannelIRC::userJoin(ClientIRC *const &newbie)
	{
		for (std::vector<ClientIRC*>::const_iterator it = this->getClientList().begin(); it != this->getClientList().end(); ++it) {
			ClientIRC *target = *it;
			if (clientExists(target, _ope_list)) {
				target->sendMessage(newbie, RPL_NAMREPLY_OP(newbie->getNick(), this->getName(), target->getNick()));
			} else {
				target->sendMessage(newbie, RPL_NAMREPLY(newbie->getNick(), this->getName(), target->getNick()));
			}
		}
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

	void	ChannelIRC::clearUser(ClientIRC *const &to_clear) {
		std::vector<ClientIRC *>::iterator ite;

		ite = ClientIterator(to_clear, _ope_list);
		if (ite != _ope_list.end())
			this->_ope_list.erase(ite);

		ite = ClientIterator(to_clear, _ban_list);
		if (ite != _ban_list.end())
			this->_ban_list.erase(ite);

		ite = ClientIterator(to_clear, _client_list);
		if (ite != _client_list.end())
			this->_client_list.erase(ite);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::vector<ClientIRC *>::iterator ClientIterator
		(ClientIRC *const &to_find, std::vector<ClientIRC *> &to_check)
	{
		std::vector<ClientIRC *>::iterator ite = to_check.begin();
		while (ite != to_check.end() && (*ite)->getId() != to_find->getId())
			ite++;
		return ite;
	}

	bool	clientExists(ClientIRC *const &to_add, std::vector<ClientIRC *> &to_check)
	{
		std::vector<ClientIRC *>::iterator ite = to_check.begin();
		while (ite != to_check.end() && (*ite)->getId() != to_add->getId())
			ite++;
		if (ite == to_check.end())
			return (false);
		return (true);
	}

}
