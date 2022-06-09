//
// Created by Hajar Sabir on 6/9/22.
//

#ifndef IRCSERV_BOT_HPP
#define IRCSERV_BOT_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>

class Bot {
private:
	int			_sock;
	int			_port;
	std::string	_host;
	std::string	_pass;
	std::string	_buff;

	struct sockaddr_in			_server_address;

public:
	Bot(const std::string &host, const int &port, const std::string &password);
	~Bot();

	void	run();
	void	send_cmd(std::string args);
	void	e_listener();
};

#endif //IRCSERV_BOT_HPP
