//
// Created by Tadeusz Kondracki on 4/1/22.
//

#include "User.hpp"

User::User():
	is_registered(false),
	nickname("tbd"),
	_is_op(false)
{
	_channels = std::vector<std::string>();
}

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
	nickname = rhs.nickname;
	_is_op = rhs._is_op;
	_channels = rhs._channels;
	return *this;
}
