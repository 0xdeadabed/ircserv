//
// Created by Tadeusz Kondracki on 3/29/22.
//

#include "Channel.hpp"
#include "Client.hpp"

Channel::Channel(): _name("general")
{
	_members = std::vector<Client *>();
}

Channel::Channel(const std::string &name, const std::string &password, Client *admin)
	: _name(name), _password(password), _admin(admin) {

}

Channel::Channel(Channel const &inst)
{
	*this = inst;
}

Channel::~Channel()
{

}

Channel &Channel::operator=(Channel const &rhs)
{
	_name = rhs._name;
	_members = rhs._members;
	_password = rhs._password;
	_admin = rhs._admin;
	return *this;
}

std::string	Channel::getName() {
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

void	Channel::addUser(Client *user) {
//	if (user->is_registered())
		_members.push_back(user);
//	else {
//		//todo
//	}
}

void	Channel::joinMessage(std::string const &message) {
	for(ch_it it = _members.begin(); it != _members.end(); it++)
		(*it)->send_msg(message);
		//TODO: Send a message to everyone
}
