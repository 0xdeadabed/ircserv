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
	std::string	_chan;

	struct sockaddr_in			_server_address;
	std::vector<std::string>	args;

public:
	Bot(const std::string &host, const int &port, const std::string &password, const std::string channel);
	~ Bot();

	void	run();
	void	send_cmd(std::string args) const;
	void	e_listener();
	void	read_msg(const std::string &message);
//	void	replay_cmd(const std::string &src, const std::string &cmd, std::vector<std::string> arg) const;
	void	replay_cmd(const std::string &src, std::string &cmd, std::vector<std::string> arg);
	void	split(std::string &buffer);
	static void	dcc_send(const std::vector<std::string>& arg);


	static void	server_listen(std::vector<std::string> arg);

	static void	client_send(std::vector<std::string> arg);
};

#endif //IRCSERV_BOT_HPP
