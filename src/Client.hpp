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
#include "Server.hpp"
#include <fstream>


class Server;
//class Channel;

#define READ_LEN 1024
#ifdef IRC_LOG
#define log_sent(msg) { \
	std::time_t result = std::time(NULL); \
	std::cout << " <<< " << std::asctime(std::localtime(&result)) << "dest:" << ip_address << "\nsent:" << (msg) << std::flush; \
}
#elif defined(IRC_LOG_FILE)
#define log_sent(msg) { \
	std::time_t result = std::time(NULL); \
    std::ofstream file; \
	file.open("irc_log", std::ios::out | std::ios::app);\
	file << " <<< " << std::asctime(std::localtime(&result)) << "dest:" << ip_address << "\nsent:" << (msg) << std::flush; \
	file.close();\
}
#else
#define log_sent(msg)
#endif

#ifdef IRC_LOG
#define log_received(msg) { \
	std::time_t result = std::time(NULL); \
	std::cout << " >>> " << std::asctime(std::localtime(&result)) << "origin:" << ip_address << "\nreceived:" << (msg) << std::flush; \
}
#elif defined(IRC_LOG_FILE)
#define log_received(msg) { \
	std::time_t result = std::time(NULL); \
    std::ofstream file; \
	file.open("irc_log", std::ios::out | std::ios::app);\
	file << " >>> " << std::asctime(std::localtime(&result)) << "origin:" << ip_address << "\nreceived:" << (msg) << std::flush; \
	file.close();\
}
#else
#define log_received(msg)
#endif

typedef std::vector<pollfd>::iterator cpiterator;

struct s_user {
	std::string nickname;
	std::string username;
	std::string real_name;
	std::string hostname;
	std::string mode;
	std::string unused;
	bool		is_oper;
	bool		is_registered;
	bool		is_logged;
	bool		wlc;
	Channel *_channel;
} typedef user;

class Client {
public:
	Client(int listen_fd, Server &serv);
	Client(Client const &inst);
	Client(Server &serv, int connect_fd);
	~Client();

	class clientException : public std::exception {
	};

	Client &operator=(Client const &rhs);

	Channel	*getChannel() { return _user._channel; }
	int get_fd() const;
	std::string getNickname() const { return _user.nickname; };
	std::string	getUsername() const { return _user.username; };
	void send_msg(const std::string &msg);
	void manage_events(short revents);

	bool is_registered() const;
	bool is_queue_empty();
	bool _quit;

	time_t get_last_activity() const;

private:
	friend class TestManager;

	int _fd;
	struct sockaddr_in _addr;
	socklen_t _addr_len;
	std::string ip_address;
	user _user;
	std::string _buffer;
	std::time_t _last_activity;
	std::vector<std::string> _queue;
	Server &host;

	//irc ids
	enum irc_command {
		NICK,
		USER,
		PASS,
		JOIN,
		QUIT,
		LIST,
		PART,
		PRIVMSG,
		CAP,
		UNKNOWN
	};

	void	read_inp();
	void	send_out();
	void	check_buff();
	void	manage_command(const std::string& cmd);
	void	parse_cmd(std::string str, irc_cmd *cmd);
	void	exec_cmd(const irc_cmd &cmd);
	static	irc_command get_cmd_id(const std::string &cmd);
	void	joinChannel(Channel *channel);
	void	leaveChannel();

	//irc_cmds
	void	nick(std::vector<std::string> args);
	void	userName(std::vector<std::string> args);
	void	join(std::vector<std::string> args);
	void	quit();
	void	pass(std::vector<std::string> args);
	void	list(Client *c);
	void	part(std::vector<std::string> args);
	void	pmsg(std::vector<std::string> args);
	void	cap() { return;};

};

#endif //IRCSERV_CLIENT_HPP
