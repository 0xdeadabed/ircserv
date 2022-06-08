//
// Created by Tadeusz Kondracki on 3/29/22.
//

#include "Channel.hpp"
#include "Client.hpp"
#include <unistd.h>
#include <algorithm>

Channel::Channel(const std::string &name, const std::string &password, Client *admin)
	: _name(name), _password(password), _admin(admin) {

}

Channel::Channel(Channel const &inst) {
	*this = inst;
}

Channel::~Channel() {
}

Channel &Channel::operator=(Channel const &rhs) {
	_name = rhs._name;
	_members = rhs._members;
	_password = rhs._password;
	_admin = rhs._admin;
	return *this;
}

std::string Channel::getName() {
	return _name;
}

std::vector<std::string> Channel::getNicknames() {
	std::vector<std::string> nicknames;

	for (ch_it it = _members.begin(); it != _members.end(); it++) {
		Client *user = it.operator*();
		nicknames.push_back((_admin == user ? "@" : "") + (*it)->getNickname());
	}

	return nicknames;
}

void Channel::addUser(Client *user) {
	if (user->is_registered())
		_members.push_back(user);
	else {
		//todo
		return;
	}
}

void Channel::setAdmin(Client *admin) {
	_admin = admin;
}

void Channel::setOperator(Client *op) {
	_operators.push_back(op);
}

void Channel::removeOperator(Client *client) {
	_operators.erase(std::remove(_operators.begin(), _operators.end(), client), _operators.end());
}

bool Channel::isAdmin(Client *client) {
	return (client == _admin);
}

bool Channel::isOperator(Client *client) {
	if (_operators.empty())
		return false;
	for (ch_it it = _operators.begin(); it != _operators.end(); it++) {
		if (*it == client)
			return true;
	}
	return false;
}

bool Channel::isInChannel(Client *client) {
	for (ch_it it = _members.begin(); it != _members.end(); it++)
		if (*it == client)
			return true;
	return false;
}

void Channel::sendMessage(std::string const &message, Client *sender) {
	for (ch_it it = _members.begin(); it != _members.end(); it++)
	{
		if (*it == sender)
			continue;
		(*it)->send_msg(message);
	}
}

void	Channel::removeUser(Client *client) {
	_members.erase(std::remove(_members.begin(), _members.end(), client), _members.end());

	if (_members.empty())
		return;

	if (_admin == client) {
		if (!_operators.empty()) {
			_admin = _operators.begin().operator*();
			return;
		}
		_admin = _members.begin().operator*();
	}
}

Client *Channel::getClient(std::string name) {
	for (ch_it it = _members.begin(); it != _members.end(); it++) {
		if ((*it)->getUsername() == name || (*it)->getNickname() == name)
			return *it;
	}
	return NULL;
}

void	Channel::setMaxSize(bool con, size_t size = 0) {
	this->_l_active = con;
	this->_maxSize = size;
}

size_t	Channel::getMaxSize() {
	return this->_maxSize;
}

size_t	Channel::getClientsNumber() {
	return _members.size();
}

bool	Channel::isMaxMode() {
	return this->_l_active;
}
