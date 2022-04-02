//
// Created by Tadeusz Kondracki on 3/29/22.
//

#ifndef IRCSERV_CLIENT_HPP
#define IRCSERV_CLIENT_HPP

#include <iostream>
#include <vector>
#include <netinet/in.h>
#include <ctime>
#include "irc_cmd.h"
#include "poll.h"
#include "Channel.hpp"


class Server;
//class Channel;


#define READ_LEN 1024

typedef	std::vector<pollfd>::iterator	cpiterator;

struct s_user{
	std::string nickname;
	std::string name;
	std::string real_name;
	std::string hostname;
	bool		is_oper;
	bool		is_registered;
	//todo replace by a channel perms class?
	Channel 	*_channel;
} typedef user;

class Client
{
public:
	Client(int listen_fd, Server &serv);
	Client(Client const &inst);
	~Client();

	class clientException: public std::exception {};

	Client &operator=(Client const &rhs);

	int		get_fd() const;
	std::string	getNickname() { return _user.nickname; };
	void setNickname(const std::string  &nickname) { _user.nickname = nickname; }
	void	manage_events(short revents);
	bool	is_registered() const;
	time_t	get_last_activity() const;
	void	send_msg(const std::string& msg);
	bool	is_queue_empty();

	bool						_quit;

private:
	int							_fd;
//	cpiterator					_pollfd;
	
	struct sockaddr_in			_addr;
	socklen_t					_addr_len;
	std::string					ip_address;
	user						_user;
	std::string					_buffer;
	std::time_t					_last_activity;
	std::vector<std::string>	_queue;
	Server						&host;

	//irc ids
	enum irc_command{
		NICK,
		USER,
		PASS,
		JOIN,
		QUIT,
		UNKNOWN
	};

	void				read_inp();
	void				send_out();
	void				check_buff();
	void				manage_command(std::string cmd);
	void				parse_cmd(std::string str, irc_cmd *cmd);
	void				exec_cmd(const irc_cmd& cmd);
	static irc_command	get_cmd_id(const std::string& cmd);

	//irc_cmds
	void	nick();
	void	user();
	void	join(Channel *channel, Client *client);
	void	quit();
	void	pass();

};

#endif //IRCSERV_CLIENT_HPP
