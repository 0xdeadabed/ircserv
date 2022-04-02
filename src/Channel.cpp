//
// Created by Tadeusz Kondracki on 3/29/22.
//

#include "Channel.hpp"

Channel::Channel(): _name("general")
{
	_members = std::vector<User *>();
}

Channel::Channel(const std::string &name, const std::string &password, User *admin)
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
	return *this;
}

std::string	Channel::getName() {
	return _name;
}

std::vector<std::string> Channel::getNicknames() {
	std::vector<std::string> nicknames;

	for (ch_it it = _members.begin(); it != _members.end(); it++) {
		User *user = it.operator*();
		nicknames.push_back((_admin == user ? "@" : "") + (*it)->getNickname());
	}

	return nicknames;
}

void	Channel::addUser(User *user) {
	_members.push_back(user);
}

void	Channel::broadJoin(std::string &message) {
	//for(ch_it it = _members.begin(); it != _members.end(); it++)
		//TODO: Send a message to everyone
}
