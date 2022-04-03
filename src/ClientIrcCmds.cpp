//
// Created by Tadeusz Kondracki on 4/2/22.
//

#include "Client.hpp"
#include "messages.hpp"
#include "Server.hpp"

//class Server;

void	Client::nick(std::vector<std::string> args){
	std::cout << "got nick command" << std::endl;
	(void)args;
	// TODO fix the args parsing
}

void	Client::user(){
	std::cout << "got user command" << std::endl;
}

void	Client::join(Channel *channel, Client *client){
	std::string	admin;

	channel->addUser(client);
	_user._channel = channel;
	channel->joinMessage(RPL_JOIN(_user.nickname, channel->getName()));
	std::vector<std::string>	nicknames = channel->getNicknames();
}

void	Client::quit(){
	_quit = true;
	std::cout << "QUIT" << std::endl;
}

void	Client::pass(){
	std::cout << "got pass command" << std::endl;
}

void	Client::list(Client *c){
	host.getChannels(c);
}
