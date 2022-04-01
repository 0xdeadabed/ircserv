//
// Created by Tadeusz Kondracki on 3/29/22.
//

#ifndef IRCSERV_CLIENT_HPP
#define IRCSERV_CLIENT_HPP

#include <iostream>
#include <vector>
#include <netinet/in.h>
#include <ctime>
#include "User.hpp"
#include "irc_cmd.h"
#include "Server.hpp"

#define READ_LEN 1024

class Client
{
public:
	Client(int listen_fd, Server &host);
	Client(Client const &inst);
	~Client();

class clientException: public std::exception {};

	Client &operator=(Client const &rhs);

	int		get_fd() const;
	void	manage_events(short revents);

private:
	int							_fd;
	struct sockaddr_in			_addr;
	socklen_t					_adrr_len;
	std::string					ip_address;
	User						_user;
	std::string					_buffer;
	std::time_t					_last_activity;
	std::vector<std::string>	_queue;
	Server						&host;

	void			read_inp();
	void			send_out();
	void			check_buff();
	void			manage_command(std::string cmd);
	void			parse_cmd(std::string str, irc_cmd *cmd);
	std::string		exec_cmd(const irc_cmd& cmd);
};

#endif //IRCSERV_CLIENT_HPP
