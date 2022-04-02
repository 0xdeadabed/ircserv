//
// Created by Tadeusz Kondracki on 4/1/22.
//

#ifndef IRCSERV_USER_HPP
#define IRCSERV_USER_HPP

#include <vector>
#include <iostream>
#include "Channel.hpp"

class Channel;

class User
{
public:
	User();
	User(User const &inst);
	~User();

	User &operator=(User const &rhs);

	std::string	getNickname(void);
	void	joinChannel(Channel *channel);

private:
	bool			is_registered;
	std::string		_nickname;
	std::string		_username;
	bool 			_is_op;
	Channel			*_channel;
};


#endif //IRCSERV_USER_HPP
