//
// Created by Tadeusz Kondracki on 4/1/22.
//

#include "User.hpp"

User::User():
	is_registered(false),
	_nickname("tbd"),
	_is_op(false) {}

User::User(User const &inst)
{
	*this = inst;
}

User::~User()
{

}

User &User::operator=(User const &rhs)
{
	is_registered = rhs.is_registered;
	_nickname = rhs._nickname;
	_is_op = rhs._is_op;
	_channel = rhs._channel;
	return *this;
}

std::string	User::getNickname(void) {
	return _nickname;
}

void	User::joinChannel(Channel *channel) {
	channel->addUser(this);
	_channel = channel;
}
