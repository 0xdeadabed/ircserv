//
// Created by Tadeusz Kondracki on 3/29/22.
//

#include "Channel.hpp"

Channel::Channel(): name("general")
{
	_members = std::vector<std::string>();
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
	name = rhs.name;
	_members = rhs._members;
	return *this;
}
