//
// Created by Tadeusz Kondracki on 3/29/22.
//

#ifndef IRCSERV_CHANNEL_HPP
#define IRCSERV_CHANNEL_HPP

#include <iostream>
#include <vector>

class Client;
//class User;
class Channel;



class Channel
{
	typedef	std::vector<Client *>::iterator	ch_it;
public:
//	Channel();
	Channel(const std::string &name, const std::string &password, Client *admin);
	Channel(Channel const &inst);
	~Channel();

	Channel &operator=(Channel const &rhs);

	std::string	getName();
	std::vector<std::string> getNicknames();
	void	addUser(Client *user);
	void	joinMessage(std::string const &message);
	std::string getPassword() const { return _password; };

private:
	std::string				_name;
	std::string				_password;
	std::vector<Client *>	_members;
	Client					*_admin;
};


#endif //IRCSERV_CHANNEL_HPP
