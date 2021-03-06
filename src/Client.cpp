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
#include "messages.hpp"
//#include "Server.hpp"

//for tests
Client::Client(Server &serv, int connect_fd): _quit(false), _fd(connect_fd), _addr(), _addr_len(), host(serv) {
	ip_address = "127.0.0.1";
	_buffer = std::string();
	_last_activity = std::time(NULL);
	_user.is_registered = false;
	_user.is_oper = false;
	_user.wlc = false;
}

Client::Client(int listen_fd, Server &serv) : _quit(false), _addr(), _addr_len(), host(serv) {
	_addr_len = sizeof(_addr);
	std::memset(&_addr, 0, _addr_len);
	_fd = accept(listen_fd, (sockaddr *) &_addr, &_addr_len);
	if (_fd < 0)
		throw std::runtime_error("Error: couldn't connect a client");
	char buff[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &_addr.sin_addr, buff, INET_ADDRSTRLEN);
	ip_address = std::string(buff);
	_buffer = std::string();
	_last_activity = std::time(NULL);
	_user.is_registered = false;
	_user.is_logged = false;
	_user.is_oper = false;
	_user._channel = NULL;
	_user.wlc = false;
}

Client::Client(Client const &inst) : _quit(), _fd(), _addr(), _addr_len(), _last_activity(), host(inst.host) {
	*this = inst;
}

Client::~Client() {
	close(_fd);
	log_else("closing client buff state:[" + _buffer + "]\n");
	//delete _user._channel;
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

// Getters
std::string Client::getPrefix() const {
	return _user.nickname + (_user.username.empty() ? "" : "!" + _user.username) + (_user.hostname.empty() ? "" : "@" + _user.hostname);
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
	log_sent(_queue[0]);
	this->_queue.erase(this->_queue.begin());
}

void Client::check_buff() {
	size_t pos;
	std::string temp;

	while ((pos = _buffer.find("\r\n")) != std::string::npos) {
		if (pos == 0)
			temp = "";
		else
			temp = _buffer.substr(0, pos);
		_buffer.erase(0, pos + 2);
		log_received(temp + "\r\n");
		this->manage_command(temp);
	}
}

void Client::manage_command(const std::string& cmd) {
	irc_cmd parsed_cmd;

	if (cmd.empty())
		return;
	this->parse_cmd(cmd, &parsed_cmd);

	this->exec_cmd(parsed_cmd);
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
	size_t n = str.find(' ');
	cmd->cmd = str.substr(0, n);
	if (n == std::string::npos){
		if (!last_arg.empty())
			cmd->args.push_back(last_arg);
		return;
	}
	str.erase(0, n + 1);
	if (str.find(':') != std::string::npos) {
		last_arg = str.substr(str.find(':') + 1);
		str.erase(str.find(':'), std::string::npos);
		if (str.length() - 1 == ' ')
			str = str.substr(0, str.size() - 1);
            //str.pop_back();
	}
	split(str, &(cmd->args));
	if (!last_arg.empty())
		cmd->args.push_back(last_arg);
}

void Client::exec_cmd(const irc_cmd &cmd) {
	switch (get_cmd_id(cmd.cmd)) {
		case NICK: nick(cmd.args); break; //Done
		case USER: userName(cmd.args); break; //Done
		case PASS: pass(cmd.args); break; //Done
		case JOIN: join(cmd.args); break; //Done
		case QUIT: quit(); break; //Done
		case LIST: list(this); break; //Done
		case PART: part(cmd.args); break; //Done
		case PRIVMSG: pmsg(cmd.args); break; //Done
		case CAP: cap(); break; // Done
		case PING: ping(cmd.args); break;
		case PONG: pong(cmd.args); break;
		case KICK: kick(cmd.args); break;
		case MODE: mode(cmd.args); break;
		case NOTICE: notice(cmd.args); break;
		case UNKNOWN:
			_queue.push_back(ERR_UNKNOWNCOMMAND(this->getNickname(), cmd.cmd));
			break;
		default:
			std::cout << "dropped command " << cmd.cmd << std::endl;
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
	if (cmd == "PART")
		return PART;
	if (cmd == "PRIVMSG")
		return PRIVMSG;
	if (cmd == "CAP")
		return CAP;
	if (cmd == "PING")
		return PING;
	if (cmd == "PONG")
		return PONG;
	if (cmd == "MODE")
		return MODE;
	if (cmd == "WHOIS")
		return WHOIS;
	if (cmd == "KICK")
		return KICK;
	if (cmd == "NOTICE")
		return NOTICE;
	return UNKNOWN;
}

bool Client::is_registered() const {
	return _user.is_registered;
}

std::time_t Client::get_last_activity() const {
	return _last_activity;
}

std::time_t Client::get_last_ping() const {
	return _last_ping;
}

void	Client::set_last_activity(std::time_t act) {
	_last_activity = act;
}

void	Client::set_last_ping(std::time_t ping) {
	_last_ping = ping;
}

void Client::send_msg(const std::string &msg) {
	_queue.push_back(msg);
}

bool Client::is_queue_empty() {
	return _queue.empty();
}

void Client::joinChannel(Channel *channel) {
	std::vector<std::string> nicknames;
	std::string admins;

	channel->addUser(this);
	_user._channel = channel;
	channel->sendMessage(CNF_JOIN(_user.nickname, channel->getName()), NULL);
	nicknames = channel->getNicknames();
	for (std::vector<std::string>::iterator it = nicknames.begin(); it != nicknames.end(); it++)
		admins.append(it.operator*() + " ");
	this->send_msg(RPL_NAMREPLY(_user.nickname, channel->getName(), admins));
	this->send_msg(RPL_ENDOFNAMES(_user.nickname, channel->getName()));
}

void	Client::leaveChannel() {
	if (!_user._channel)
		return;

	_user._channel->removeUser(this);
}

std::string Client::getNickname() const { return _user.nickname; }
std::string	Client::getUsername() const { return _user.username; }

std::string	Client::getAddress() const{
	return ip_address;
}
