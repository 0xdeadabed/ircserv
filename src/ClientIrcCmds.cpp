//
// Created by Tadeusz Kondracki on 4/2/22.
//

#include <string>
#include "Client.hpp"
#include "messages.hpp"
#include <cstring>
#include <string.h>

void Client::nick(std::vector<std::string> args) {
	//TODO: maybe register if only user is logged
	if (args.size() != 1) {
		this->send_msg(ERR_NONICKNAMEGIVEN);
		return;
	}
	if (host.getClient(args[0])) {
		this->send_msg(ERR_NICKNAMEINUSE(args[0]));
		return;
	}
//	if (!_user.is_registered) {
//		_user.nickname = args[0];
//		_user.username = args[0];
//		_user.is_registered = true;
//		_queue.push_back(RPL_WELCOME(_user.nickname, _user.username, _user.hostname));
//	} else {
		if (!_user.nickname.empty())
			_queue.push_back(CH_NICK(_user.nickname, args[0]));
//		else
//			_queue.push_back(RPL_WELCOME(_user.nickname, _user.username, _user.hostname));
		_user.nickname = args[0];
//	}
}

void Client::userName(std::vector<std::string> args) {
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

void Client::join(std::vector<std::string> args) {
	if (!_user.is_registered) {
		//TODO: send a nice message but now I'm too lazy to do it
		return;
	}

	if (args.empty()) {
		this->send_msg(ERR_NEEDMOREPARAMS("JOIN"));
		return;
	}

	std::string password = args.size() > 1 ? args[1] : "";
	std::string channel_name = args[0];
	Channel *channel = host.getChannels(channel_name);
	if (!channel)
		channel = host.create_channel(channel_name, password, this);
	if (channel->getPassword() != password) {
		this->send_msg(ERR_BADCHANNELKEY(args[0]));
		return;
	}
	this->joinChannel(channel);
}

void Client::quit() {
	_quit = true;
	this->leaveChannel();
	std::cout << "QUIT" << std::endl;
}

void Client::pass(std::vector<std::string> args) {
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

void Client::list(Client *c) {
	host.listChannel(c);
}

void	Client::part(std::vector<std::string> args) {
	if (args.empty()) {
		this->send_msg(ERR_NEEDMOREPARAMS("PART"));
		return;
	}
	std::string	channel_name = args[0];

	Channel	*channel = host.getChannels(channel_name);
	if (!channel) {
		this->send_msg(ERR_NOSUCHCHANNEL(channel_name));
		return;
	}
	if (!this->getChannel() || this->getChannel()->getName() != channel_name) {
		this->send_msg(ERR_NOTONCHANNEL(channel_name));
		return;
	}

	this->leaveChannel();
}
