//
// Created by Tadeusz Kondracki on 4/2/22.
//

#include <string>
#include "Client.hpp"
#include "messages.hpp"
#include <cstring>
#include <string.h>
//#include "Server.hpp"

//class Server;

void	Client::nick(std::vector<std::string> args){
	//TODO: maybe register if only user is logged
	if (args.size() != 1) {
		this->send_msg(ERR_NONNICK);
		return;
	}
	if (host.getClient(args[0])) {
		this->send_msg(ERR_NICKNAMEINUSE);
		return;
	}
	if (!_user.is_registered) {
		_user.nickname = args[0];
		_user.username = args[0];
		_user.is_registered = true;
		_queue.push_back(RPL_WELCOME(_user.nickname, _user.username, _user.hostname));
	} else {
		if (!_user.nickname.empty())
			_queue.push_back(CH_NICK(_user.nickname, args[0]));
		else
			_queue.push_back(RPL_WELCOME(_user.nickname, _user.username, _user.hostname));
		_user.nickname = args[0];
	}
}

void	Client::userName(std::vector<std::string> args){
	//TODO: maybe register if only user is logged
	if (_user.is_registered) {
		this->send_msg(ERR_ALREADYREGISTERED);
		return;
	}
	if (args.size() < 3) {
		this->send_msg(ERR_NEEDMOREPARAMS("USER"));
		return;
	}
	_user.is_registered = true;
	_user.username = args[0];
	_user.hostname = args[1];
	_user.real_name = args[2];
//	_queue.push_back(RPL_WELCOME(_user.username, _user.real_name, _user.hostname));

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

void	Client::pass(std::vector<std::string> args){
	if (_user.is_registered) {
		this->send_msg(ERR_ALREADYREGISTERED);
		return;
	}

	if (args.empty()) {
		this->send_msg(ERR_NEEDMOREPARAMS("PASS"));
		return;
	}

	if (args[0].compare(host.getPass())) {
		this->send_msg(ERR_PASSWDMISMATCH);
		return;
	}

	_user.is_logged = true;
}

void	Client::list(Client *c){
	host.getChannels(c);
}
