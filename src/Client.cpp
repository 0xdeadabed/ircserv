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

Client::Client(int listen_fd):
	nickname("tbd"),
	_is_op(false)
{
	char	ip[INET_ADDRSTRLEN + 1];

	_fd = accept(listen_fd, (sockaddr *) &_addr, &_adrr_len);
	if (_fd < 0)
		throw clientException();
	inet_ntop(AF_INET, &_addr, ip, INET_ADDRSTRLEN);
	ip_address = ip;
	_channels = std::vector<std::string>();
	_buffer = std::string();
	_last_activity = std::time(NULL);
	_queue = std::vector<std::string>();
}

Client::Client(Client const &inst)
{
	*this = inst;
}

Client::~Client()
{

}

Client &Client::operator=(Client const &rhs)
{
	_fd = rhs._fd;
	_addr = rhs._addr;
	_adrr_len = rhs._adrr_len;
	nickname = rhs.nickname;
	ip_address = rhs.ip_address;
	_channels = rhs._channels;
	_buffer = rhs._buffer;
	_is_op = rhs._is_op;
	_last_activity = rhs._last_activity;
	_queue = rhs._queue;
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

	//bzero(buffer, READ_LEN + 1);
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
	if ((pos = _buffer.find("\n")) != std::string::npos){
		if (pos == 0)
			temp = "";
		else
			temp = _buffer.substr(0, pos);
		_buffer.erase(0, pos + 1);
		this->exec_cmd(temp);
	}
}

void Client::exec_cmd(std::string cmd)
{
	std::time_t stamp = std::time(NULL);
	std::string answer = "[";

	answer.append(std::asctime(std::localtime(&stamp))); //.pop_back(); pop_back is c++11 feature
	std::ostringstream	convert;	//Stream used for the conversion -> c++98
	convert << _fd;
	answer.append("] fd: " + convert.str() + " received message:\n");
	_queue.push_back(answer + cmd + "\n--------\n");
}
