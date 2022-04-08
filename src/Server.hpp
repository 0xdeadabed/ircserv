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
#include "irc_cmd.h"

# ifndef IRC_SERV_PORT
# 	define IRC_SERV_PORT 6697
# endif

class	Client;
class	Channel;

class Server {
public:
	Server();

	Server(const std::string &port, const std::string &password);

	//Server(Server const &inst);
	~Server();

	void		loop();
	void		listChannel(Client *c);
	Channel		*create_channel(const std::string &name, const std::string &password, Client *client);
	Channel		*getChannels(const std::string &name);
	Client		*getClient(const std::string &nickname);
	std::string	getPass() const { return _password; };
private:
	friend class TestManager;

	typedef std::vector<pollfd>::iterator piterator;

	struct pollfd listen_fd;
	const std::string _port;
	const std::string _password;
	struct sockaddr_in _server_address;
	std::vector<struct pollfd> _watchlist;
	std::map<int, Client *> _clients;
	std::vector<Channel *> _channels;

	void 		add_client();

	void 		disconnect_timeouts();

	void 		delete_client(Client *c);
};

#endif //FT_IRC_SERVER_HPP
