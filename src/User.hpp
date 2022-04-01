//
// Created by Tadeusz Kondracki on 4/1/22.
//

#ifndef IRCSERV_USER_HPP
#define IRCSERV_USER_HPP

#include <vector>
#include <iostream>

class User
{
public:
	User();
	User(User const &inst);
	~User();

	User &operator=(User const &rhs);

private:
	bool						is_registered;
	std::string					nickname;
	bool 						_is_op;
	std::vector<std::string>	_channels;
};


#endif //IRCSERV_USER_HPP
