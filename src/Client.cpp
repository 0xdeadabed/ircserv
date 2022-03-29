//
// Created by Tadeusz Kondracki on 3/29/22.
//

#include "Client.hpp"

Client::Client()
{
	//default values
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
	_channels = rhs._channels;
	_buffer = rhs._buffer;
}

std::iostream &operator<<(std::iostream &io, Client const & client){
	io << "client todo?";
	return io;
}
