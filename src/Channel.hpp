//
// Created by Tadeusz Kondracki on 3/29/22.
//

#ifndef IRCSERV_CHANNEL_HPP
#define IRCSERV_CHANNEL_HPP

#include <iostream>
#include <vector>

class Client;
class User;

typedef	std::vector<User *>::iterator	ch_it;

class Channel
{
public:
	Channel();
	Channel(const std::string &name, const std::string &password, User *admin);
	Channel(Channel const &inst);
	~Channel();

	Channel &operator=(Channel const &rhs);

	std::string	getName();
//	std::vector<std::string> getNicknames();
	void	addUser(Client *user);
//	void	joinMessage(std::string &message);

private:
	std::string				_name;
	std::string				_password;
	std::vector<Client *>	_members;
	User					*_admin;
};


#endif //IRCSERV_CHANNEL_HPP
