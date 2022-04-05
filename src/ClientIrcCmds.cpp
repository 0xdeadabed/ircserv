//
// Created by Tadeusz Kondracki on 4/2/22.
//

#include <string>
#include "Client.hpp"
#include "messages.hpp"
//#include "Server.hpp"

//class Server;

void	Client::nick(std::vector<std::string> args){
	if (args.size() != 1) {
		this->send_msg(ERR_NONNICK);
		return;
	}
	if (host.getClient(args[0])) {
		this->send_msg(ERR_USEDNICK);
	}
	_user.nickname = args[0];//std::to_string(_fd);
	_user.is_registered = true;
	this->send_msg(RPL_WELCOME(_user.nickname));
}

void	Client::userName(){
	std::cout << "got user command" << std::endl;
}

void	Client::join(Client *client, std::vector<std::string> cmd){
//	std::string	admin;
	(void )client;
	(void )cmd;
//	channel->addUser(this);
//	_user._channel = channel;
//	channel->joinMessage(RPL_JOIN(_user.nickname, channel->getName()));
//	std::vector<std::string>	nicknames = channel->getNicknames();
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
