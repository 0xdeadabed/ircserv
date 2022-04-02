//
// Created by Tadeusz Kondracki on 4/2/22.
//

#include "Client.hpp"
#include "messages.hpp"

void	Client::nick(){
	std::cout << "got nick command" << std::endl;
}

void	Client::user(){
	std::cout << "got user command" << std::endl;
}

void	Client::join(Channel *channel, Client *client){
	std::string	admin;

	//	std::cout << "got join command: " << channel->getName() << std::endl;
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
