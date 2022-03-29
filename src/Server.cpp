//
// Created by Tadeusz Kondracki on 3/29/22.
//

#include <sys/socket.h>
#include "Server.hpp"

Server::Server(unsigned int port, std::string password):
	_port(port),
	_password(password)
{
	struct pollfd listen_fd;

	listen_fd.fd = socket(AF_INET, SOCK_STREAM, 0);
	listen_fd.events = POLLIN;
	listen_fd.revents = 0;
	if (listen_fd.fd < 0)
		throw socketCreationException();

	bzero(&_server_address, sizeof(_server_address));
	_server_address.sin_family = AF_INET;
	_server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	_server_address.sin_port = htons(_port);

	if (bind(listen_fd.fd, (sockaddr *) &_server_address, sizeof(_server_address)) < 0)
		throw socketBindException();
	if (listen(listen_fd.fd, 10) < 0)
		throw socketListenException();

	_watchlist = std::vector<struct pollfd>();
	_watchlist.push_back(listen_fd);
	_clients = std::map<int, Client>();
	_channels = std::vector<Channel>();
}

Server::Server(Server const &inst)
{
	*this = inst;
}

Server::~Server()
{

}

Server &Server::operator=(Server const &rhs)
{
	this->_port = rhs._port;
	this->_password = rhs._password;
	this->_server_address = rhs._server_address;
	this->_watchlist = rhs._watchlist;
	this->_clients = rhs._clients;
	this->_channels = rhs._channels;
}

std::iostream &operator<<(std::iostream &io, Server const & serv){
	io << "server todo?";
	return io;
}
