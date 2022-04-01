//
// Created by Tadeusz Kondracki on 3/29/22.
//

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include "Server.hpp"

Server::Server(const std::string &port, const std::string &password):
	_port(port),
	_password(password)
{
	listen_fd.fd = socket(AF_INET, SOCK_STREAM, 0);
	listen_fd.events = POLLIN;
	listen_fd.revents = 0;
	if (listen_fd.fd < 0)
		std::cout << "listen fd error" << std::endl;
	memset(&_server_address, '\0', sizeof(_server_address));
	_server_address.sin_family = AF_INET;
	_server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	_server_address.sin_port = htons(std::stoi(_port));

	if (bind(listen_fd.fd, (sockaddr *) &_server_address, sizeof(_server_address)) < 0)
		std::cout << "bind error caught" << std::endl;
	if (listen(listen_fd.fd, 10) < 0)
		std::cout << "listen error caught" << std::endl;

	_watchlist = std::vector<struct pollfd>();
	_watchlist.push_back(listen_fd);
	_channels = std::vector<Channel>();
}

Server::~Server()
{
	int	c = close(listen_fd.fd);
	std::cout << "close: " << c << std::endl;
//todo
}

bool run = true;

void	destroy(int n) {
	(void)n;
	run = false;
}

void Server::loop()
{
	signal(SIGINT, destroy);

	while (1) {
		if (run == false)
			break ;
		if (poll(_watchlist.begin().base(), _watchlist.size(), 1000) < 0)
			throw	std::runtime_error("Error in poll");
		for (piterator it = _watchlist.begin(); it != _watchlist.end() ; it++)
		{
			if (it->revents == 0)
				continue;	
			if (it == _watchlist.begin() && (it->revents & POLLIN) == POLLIN) {
				add_client();
				break;
			} 
			else if (it->revents){
				_clients[it->fd]->manage_events(it->revents);
			}
		}
		this->disconnect_timeouts();
	}
}

void	Server::add_client(void) {
	int				fd;
	struct	sockaddr_in		s_addr;
	socklen_t		s_len =	sizeof(s_addr);

	fd = accept(listen_fd.fd, (sockaddr *) &s_addr, &s_len);
	if (fd < 0)
		throw	std::runtime_error("Error: couldn't connect a client");
	//std::cout << "KEVIN" << std::endl;
	pollfd	pollfd = {fd, POLLIN, 0};
	_watchlist.push_back(pollfd);

	Client	*client = new Client(fd, _watchlist.end() - 1, inet_ntoa(s_addr.sin_addr), ntohs(s_addr.sin_port));
	_clients.insert(std::make_pair(fd, client));

	std::cout << client->getHost() << " connected" << std::endl;
}

//void Server::add_client()
//{
//	try
//	{
//		Client new_client = Client((_watchlist[0]).fd);
//		struct pollfd new_watch;
//
//		new_watch.fd = new_client.get_fd();
//		new_watch.events = POLLIN | POLLOUT;
//		new_watch.revents = 0;
//		//_clients.insert(std::pair<int, Client>(new_client.get_fd(), new_client));
//		_watchlist.push_back(new_watch);
//	}
//	catch (std::exception &e)
//	{
//		std::cout << "Error: " << e.what() << std::endl;
//	}
//}

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
