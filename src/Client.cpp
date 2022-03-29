//
// Created by Tadeusz Kondracki on 3/29/22.
//

#include <arpa/inet.h>
#include <poll.h>
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
	_last_activity = std::time(nullptr);
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
	return *this;
}

int Client::get_fd() const
{
	return _fd;
}

void Client::manage_events(short revents)
{
	if (revents & POLLIN)
		std::cout << "pollin\n";
	if (revents & POLLOUT)
		std::cout << "pollout\n";
}
