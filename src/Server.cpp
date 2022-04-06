//
// Created by Tadeusz Kondracki on 3/29/22.
//

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <signal.h>
//#include "string.h"
#include "Server.hpp"

Server::Server(const std::string &port, const std::string &password):
	_port(port),
	_password(password)
{
	listen_fd.fd = socket(AF_INET, SOCK_STREAM, 0);
	listen_fd.events = POLLIN;
	listen_fd.revents = 0;
	if (listen_fd.fd < 0)
		throw std::runtime_error("socket creation error");
	memset(&_server_address, '\0', sizeof(_server_address));
	_server_address.sin_family = AF_INET;
	_server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	_server_address.sin_port = htons(std::stoi(_port));

	if (bind(listen_fd.fd, (sockaddr *) &_server_address, sizeof(_server_address)) < 0)
		throw std::runtime_error("bind error");
	if (listen(listen_fd.fd, 10) < 0)
		throw std::runtime_error("listen error");

	_watchlist = std::vector<struct pollfd>();
	_watchlist.push_back(listen_fd);
	_clients = std::map<int, Client *>();
//	_gb = new Channel();
//	_channels.push_back(_gb);
}

Server::~Server()
{
	for (std::map<int, Client*>::iterator iter = _clients.begin(); iter != _clients.end(); ++iter)
		delete iter->second;
	int	c = close(listen_fd.fd);
	std::cout << "close: " << c << std::endl;
//	delete	_gb;
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
		// poll would use 100% of the cpu if we don't give it a break <-usleep()->
		usleep(500);
		if (poll(_watchlist.begin().base(), _watchlist.size(), 1000) < 0){
			if (errno == EINTR)
				return;
			else {
				throw	std::runtime_error("Error in poll");
			}
		}
		for (piterator it = _watchlist.begin(); it != _watchlist.end() ; it++)
		{
			if (it->revents == 0)
				continue;	
			if (it == _watchlist.begin() && (it->revents & POLLIN) == POLLIN) {
				add_client();
				break;
			} 
			else if (it->revents){
				_clients.at(it->fd)->manage_events(it->revents);
			}
		}
		this->disconnect_timeouts();
	}
}

void Server::add_client()
{
	try
	{
		Client *new_client = new Client((_watchlist[0]).fd, *this);

		pollfd	pollfd = {new_client->get_fd(), POLLIN | POLLOUT, 0};
		_clients.insert(std::pair<int, Client *>(new_client->get_fd(), new_client));
		_watchlist.push_back(pollfd);
	}
	catch (std::exception &e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}
}

void Server::disconnect_timeouts()
{
	std::time_t timestamp = std::time(nullptr);

	for( std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); )
	{
		//todo define timestam
		if(!it->second->_quit
				&& it->second->is_registered()
				&& std::difftime(timestamp, it->second->get_last_activity()) > 600){
			it->second->_quit = true;
			it->second->send_msg(":ircserv@42lausanne.ch DISCONNECTED :disconnected by timeout, bye!");
		}
		else if(!it->second->_quit
				&& !it->second->is_registered()
				&& std::difftime(timestamp, it->second->get_last_activity()) > 60){
			it->second->_quit = true;
			it->second->send_msg(":ircserv@42lausanne.ch DISCONNECTED :disconnected by timeout and no registration");
		}
		if (it->second->_quit && it->second->is_queue_empty()){
			std::map<int, Client *>::iterator to_del = it;
			it++;
			delete_client(to_del->second);
		} else {
			it++;
		}
	}
}

void Server::delete_client(Client *c){
	for (piterator i = _watchlist.begin(); i != _watchlist.end(); i++)
	{
		if(i->fd == c->get_fd()){
			_watchlist.erase(i);
			break;
		}
	}
	close(c->get_fd());
	_clients.erase(_clients.find(c->get_fd()));
	delete c;
}

// Get a list of channels at LIST command
void	Server::getChannels(Client *c) {
	std::vector<Channel *>::iterator	it;

	// Iterate over the channels and write them on the user.s fd
	for (it = _channels.begin(); it != _channels.end(); it++) {
		//std::cout << "client fd: " << c->get_fd() << std::endl;
		send(c->get_fd(), (*it)->getName().c_str(), strlen((*it)->getName().c_str()), 0);
		send(c->get_fd(), "\n", 1, 0);
	}
}

Channel	*Server::create_channel(const std::string &name, const std::string &password, Client *client) {
	Channel	*channel = new Channel(name, password, client);
	_channels.push_back(channel);
	return channel;
}

Client *Server::getClient(const std::string &nickname) {
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		if (!nickname.compare(it->second->getNickname()))
			return it->second;
	}
	return NULL;
}
