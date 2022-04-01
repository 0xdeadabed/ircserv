//
// Created by Tadeusz Kondracki on 3/29/22.
//

#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <sstream>
#include "Client.hpp"
//#include "Server.hpp"

Client::Client(int listen_fd, Server &serv): host(serv)
{
	_fd = accept(listen_fd, (sockaddr *) &_addr, &_addr_len);
	if (_fd < 0)
		throw	std::runtime_error("Error: couldn't connect a client");
	ip_address = inet_ntoa(_addr.sin_addr);
	_buffer = std::string();
	_last_activity = std::time(NULL);
	_user = User();
}

Client::Client(Client const &inst): host(inst.host)
{
	*this = inst;
}

Client::~Client()
{
	std::cout << _fd << " client destroyed " << std::endl;
	close(_fd);
}

Client &Client::operator=(Client const &rhs)
{
	_fd = rhs._fd;
	_addr = rhs._addr;
	_addr_len = rhs._addr_len;
	ip_address = rhs.ip_address;
	_user = rhs._user;
	_buffer = rhs._buffer;
	_last_activity = rhs._last_activity;
	_queue = rhs._queue;
//	host = rhs.host;
	return *this;
}

int Client::get_fd() const
{
	return _fd;
}

void Client::manage_events(short revents)
{
	if (revents & POLLIN){
		Client::read_inp();
		Client::check_buff();
	}
	if (revents & POLLOUT && !this->_queue.empty())
		Client::send_out();
}

void Client::read_inp()
{
	char buffer[READ_LEN + 1];
	int n;

	memset(buffer, '\0', READ_LEN + 1);
	while((n = read(this->_fd, buffer, READ_LEN)) == READ_LEN){
		buffer[n] = 0;
		this->_buffer.append(buffer);
	}
	if (n < 0)
		throw clientException();
	buffer[n] = 0;
	this->_buffer.append(buffer);
}

void Client::send_out()
{
	write(this->_fd, &(this->_queue[0][0]), this->_queue[0].size());
	this->_queue.erase(this->_queue.begin());
}

void Client::check_buff()
{
	size_t pos;
	std::string temp;
	//todo change
	if ((pos = _buffer.find('\n')) != std::string::npos){
		if (pos == 0)
			temp = "";
		else
			temp = _buffer.substr(0, pos);
		_buffer.erase(0, pos + 1);
		this->manage_command(temp);
	}
}

void Client::manage_command(std::string cmd)
{
	std::string answer;
	irc_cmd parsed_cmd;

	if (cmd.empty())
		return;
	this->parse_cmd(cmd, &parsed_cmd);
	answer.append("origin: " + parsed_cmd.origin + "\nCMD: " + parsed_cmd.cmd + "\nargs:");
	for (int i = 0; i < (int)parsed_cmd.args.size(); i++){
		answer.append("\n" + parsed_cmd.args[i]);
	}
	answer.append("\n");
	_queue.push_back(answer);
}

static void split(std::string str, std::vector<std::string> *args){
	size_t next;

	while((next = str.find(' ')) != std::string::npos){
		args->push_back(str.substr(0, next));
		str.erase(0, next + 1);
	}
}

void	Client::parse_cmd(std::string str, irc_cmd *cmd){
	std::string last_arg;

	if (str[0] == ':') {
		cmd->origin = str.substr(1, str.find(' ') - 1);
		str.erase(0, str.find(' ') + 1);
	}
	else {
		cmd->origin = ip_address;
	}
	cmd->cmd = str.substr(0, str.find(' '));
	str.erase(0, str.find(' ') + 1);
	if (str.find(':') != std::string::npos){
		last_arg = str.substr(str.find(':') + 1);
		str.erase(str.find(':'), std::string::npos);
	}
	split(str, &(cmd->args));
	if (!last_arg.empty())
		cmd->args.push_back(last_arg);
}

std::string	Client::exec_cmd(const irc_cmd& cmd){
	if (cmd.cmd == "DISCONNECT"){
//		host.delete_client(this);

		return ":ft_irc.com 200 :successfully disconnected";
	}
	return "Hi";
}

