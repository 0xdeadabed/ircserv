//
// Created by Tadeusz Kondracki on 3/29/22.
//

#include <sys/socket.h>
#include <unistd.h>
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
		throw socketException();

	bzero(&_server_address, sizeof(_server_address));
	_server_address.sin_family = AF_INET;
	_server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	_server_address.sin_port = htons(_port);

	if (bind(listen_fd.fd, (sockaddr *) &_server_address, sizeof(_server_address)) < 0)
		throw socketException();
	if (listen(listen_fd.fd, 10) < 0)
		throw socketException();

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
//todo
}

Server &Server::operator=(Server const &rhs)
{
	this->_port = rhs._port;
	this->_password = rhs._password;
	this->_server_address = rhs._server_address;
	this->_watchlist = rhs._watchlist;
	this->_clients = rhs._clients;
	this->_channels = rhs._channels;
	return *this;
}

void Server::loop()
{
	int ready_fds = 0;
	int				ind;

	while (true) {
		ready_fds = poll(&_watchlist[0], _watchlist.size(), 1000);
		if (ready_fds < 0)
			throw pollException();
		for (int i = ready_fds; i > 0 ; i--)
		{
			ind = this->get_next_fd();
			if (_watchlist[ind].fd == _watchlist[0].fd){
				this->add_client();
			}
			else{
				_clients[_watchlist[ind].fd].manage_events(_watchlist[ind].revents);
			}
			_watchlist[ind].revents = 0;
		}
		this->disconnect_timeouts();
		sleep(1);
	}
}

void Server::add_client()
{
	try
	{
		Client new_client = Client((_watchlist[0]).fd);
		struct pollfd new_watch;

		new_watch.fd = new_client.get_fd();
		new_watch.events = POLLIN | POLLOUT;
		new_watch.revents = 0;
		_clients.insert(std::pair<int, Client>(new_client.get_fd(), new_client));
		_watchlist.push_back(new_watch);
	}
	catch (std::exception &e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}
}

int Server::get_next_fd()
{
	for (int i = 0; i < (int)_watchlist.size(); ++i)
	{
		if (_watchlist[i].revents){
			return i;
		}
	}
	throw pollException();
}

void Server::disconnect_timeouts()
{
//	std::time_t timestamp = std::time(nullptr);
//
//	for( std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it )
//	{
//		//todo remove if timestamp to old
////		if(it->second._time)
//	}
}
