//
// Created by Tadeusz Kondracki on 3/29/22.
//

#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <sstream>
#include "sys/socket.h"
#include "Client.hpp"
//#include "Server.hpp"

//for tests
Client::Client(Server &serv, int connect_fd): _quit(false), _fd(connect_fd), _addr(), _addr_len(), host(serv) {
	ip_address = "127.0.0.1";
	_buffer = std::string();
	_last_activity = std::time(NULL);
	_user.is_registered = false;
	_user.is_oper = false;
}

Client::Client(int listen_fd, Server &serv) : _quit(false), _addr(), _addr_len(), host(serv) {
	_fd = accept(listen_fd, (sockaddr *) &_addr, &_addr_len);
	if (_fd < 0)
		throw std::runtime_error("Error: couldn't connect a client");
	ip_address = inet_ntoa(_addr.sin_addr);
	_buffer = std::string();
	_last_activity = std::time(NULL);
	_user.is_registered = false;
	_user.is_oper = false;
//	_user._channel = new Channel;
}

Client::Client(Client const &inst) : _quit(), _fd(), _addr(), _addr_len(), _last_activity(), host(inst.host) {
	*this = inst;
}

Client::~Client() {
	close(_fd);
	delete _user._channel;
}

Client &Client::operator=(Client const &rhs) {
	_fd = rhs._fd;
	_addr = rhs._addr;
	_addr_len = rhs._addr_len;
	ip_address = rhs.ip_address;
	_user = rhs._user;
	_buffer = rhs._buffer;
	_last_activity = rhs._last_activity;
	_queue = rhs._queue;
	_quit = rhs._quit;
	return *this;
}

int Client::get_fd() const {
	return _fd;
}

void Client::manage_events(short revents) {
	if (revents & POLLIN && !this->_quit) {
		Client::read_inp();
		Client::check_buff();
	}
	if (revents & POLLOUT && !this->_queue.empty())
		Client::send_out();
}

void Client::read_inp() {
	char buffer[READ_LEN + 1];
	int n;

	memset(buffer, '\0', READ_LEN + 1);
	while ((n = recv(this->_fd, buffer, READ_LEN, 0)) == READ_LEN) {
		buffer[n] = 0;
		this->_buffer.append(buffer);
	}
	if (n < 0)
		throw clientException();
	buffer[n] = 0;
	this->_buffer.append(buffer);
}

void Client::send_out() {
	if (this->_queue.empty())
		return;
	send(this->_fd, &(this->_queue[0][0]), this->_queue[0].size(), MSG_DONTWAIT);
	this->_queue.erase(this->_queue.begin());
}

void Client::check_buff() {
	size_t pos;
	std::string temp;
	//todo change
	if ((pos = _buffer.find('\n')) != std::string::npos) {
		if (pos == 0)
			temp = "";
		else
			temp = _buffer.substr(0, pos);
		_buffer.erase(0, pos + 1);
		this->manage_command(temp);
	}
}

void Client::manage_command(std::string cmd) {
//	std::string answer;
	irc_cmd parsed_cmd;

	if (cmd.empty())
		return;
	this->parse_cmd(cmd, &parsed_cmd);

	this->exec_cmd(parsed_cmd);

//	answer.append("origin: " + parsed_cmd.origin + "\nCMD: " + parsed_cmd.cmd + "\nargs:");
//	for (int i = 0; i < (int) parsed_cmd.args.size(); i++) {
//		answer.append("\n" + parsed_cmd.args[i]);
//	}
//	answer.append("\n");
//	std::cout << answer << std::endl;
//	_queue.push_back(answer);
}

static void split(std::string str, std::vector<std::string> *args) {
	size_t next;

	while ((next = str.find(' ')) != std::string::npos) {
		args->push_back(str.substr(0, next));
		str.erase(0, next + 1);
	}
	args->push_back(str);
}

void Client::parse_cmd(std::string str, irc_cmd *cmd) {
	std::string last_arg;

	if (str[0] == ':') {
		cmd->origin = str.substr(1, str.find(' ') - 1);
		str.erase(0, str.find(' ') + 1);
	} else {
		cmd->origin = ip_address;
	}
	cmd->cmd = str.substr(0, str.find(' '));
	str.erase(0, str.find(' ') + 1);
	if (str.find(':') != std::string::npos) {
		last_arg = str.substr(str.find(':') + 1);
		str.erase(str.find(':'), std::string::npos);
	}
	split(str, &(cmd->args));
	if (!last_arg.empty())
		cmd->args.push_back(last_arg);
}

void Client::exec_cmd(const irc_cmd &cmd) {
	switch (get_cmd_id(cmd.cmd)) {
		case NICK: nick(cmd.args); break;
		case USER: userName(); break;
		case PASS: pass(); break;
		case JOIN: join(this, cmd.args); break;
		case QUIT: quit(); break;
		case LIST: list(this); break;
		case UNKNOWN:
			std::cout << "unknown command" << std::endl;
			break;
		default:
			std::cout << "dropped command" << std::endl;
	}
}

Client::irc_command Client::get_cmd_id(const std::string &cmd) {
	if (cmd == "NICK")
		return NICK;
	if (cmd == "USER")
		return USER;
	if (cmd == "JOIN")
		return JOIN;
	if (cmd == "QUIT")
		return QUIT;
	if (cmd == "PASS")
		return PASS;
	if (cmd == "LIST")
		return LIST;
	return UNKNOWN;
}

bool Client::is_registered() const {
	return _user.is_registered;
}

std::time_t Client::get_last_activity() const {
	return _last_activity;
}

void Client::send_msg(const std::string &msg) {
//	std::string buffer = msg + "\r\n";
//
//	if (send(_fd, buffer.c_str(), buffer.length(), 0) < 0)
//		throw	std::runtime_error("Error: couldn't send message to a client");
	_queue.push_back(msg);
}

bool Client::is_queue_empty() {
	return _queue.empty();
}

std::vector<std::string> &Client::get_queue(){
	return _queue;
}
