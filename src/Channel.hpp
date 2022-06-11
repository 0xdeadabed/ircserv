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
	Channel(const std::string &name, const std::string &password, Client *admin);
	Channel(Channel const &inst);
	~Channel();

	Channel &operator=(Channel const &rhs);

	std::string					getName();
	std::vector<std::string>	getNicknames();
	void						addUser(Client *user);
	void						setAdmin(Client *admin);
	void						setOperator(Client *op);
	void						removeOperator(Client *admin);
	bool						isAdmin(Client *client);
	bool						isOperator(Client *client);
	bool						isInChannel(Client *client);
	void						sendMessage(std::string const &message, Client *sender);
	void						removeUser(Client *client);
	std::string					getPassword() const { return _password; };
	Client 						*getClient(std::string name);
	bool						isNMode() const { return _n; };
	void						setNMode(bool n) { this->_n = n; };
	void						setMaxSize(bool con, size_t size);
	size_t 						getMaxSize();
	size_t						getClientsNumber();
	bool						isMaxMode();
	void						setPassword(std::string k) { this->_k = k; };
    void                        cleanChannel();

private:
	std::string				_name;
	std::string				_password;
	std::vector<Client *>	_members;
	Client					*_admin;
	std::vector<Client *>	_operators;

	// Modes
	std::string				_k;
	size_t					_maxSize;
	bool					_l_active;
	bool					_n;
};


#endif //IRCSERV_CHANNEL_HPP
