//
// Created by Tadeusz Kondracki on 3/29/22.
//

#ifndef IRCSERV_CLIENT_HPP
#define IRCSERV_CLIENT_HPP

#include <iostream>
#include <vector>
#include <netinet/in.h>

#define READ_LEN 1024

class Client
{
public:
	Client(int fd = -1);
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
	std::string					nickname;
	bool 						_is_op;
	std::vector<std::string>	_channels;
	std::string					_buffer;
	std::time_t					_last_activity;
	std::vector<std::string>	_queue;

	void	read_inp();
	void	send_out();
	void	check_buff();
	void	exec_cmd(std::string cmd);
};

#endif //IRCSERV_CLIENT_HPP
