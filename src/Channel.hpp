//
// Created by Tadeusz Kondracki on 3/29/22.
//

#ifndef IRCSERV_CHANNEL_HPP
#define IRCSERV_CHANNEL_HPP

#include <iostream>
#include <vector>

class Channel
{
public:
	Channel();
	Channel(Channel const &inst);
	~Channel();

	Channel &operator=(Channel const &rhs);

private:
	std::string name;
	std::vector<std::string> _members;
};


#endif //IRCSERV_CHANNEL_HPP
