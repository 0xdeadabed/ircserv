//
// Created by Tadeusz Kondracki on 3/29/22.
//

#ifndef FT_IRC_SERVER_HPP
#define FT_IRC_SERVER_HPP

#include <iostream>
#include <poll.h>
#include <vector>
#include <map>
#include <netinet/in.h>

#include "Channel.hpp"
#include "Client.hpp"

# ifndef IRC_SERV_PORT
# 	define IRC_SERV_PORT 6697
# endif

class Server
{
public:
	Server(unsigned int port = IRC_SERV_PORT, std::string password = "1234");
	Server(Server const &inst);
	~Server();

class socketCreationException: public std::exception {};
class socketBindException: public std::exception {};
class socketListenException: public std::exception {};

	Server &operator=(Server const &rhs);

private:
	unsigned int				_port;
	std::string					_password;
	struct sockaddr_in			_server_address;
	std::vector<struct pollfd>	_watchlist;
	std::map<int, Client>		_clients;
	std::vector<Channel>		_channels;
};

std::iostream &operator<<(std::iostream &io, Server const & serv);

#endif //FT_IRC_SERVER_HPP
