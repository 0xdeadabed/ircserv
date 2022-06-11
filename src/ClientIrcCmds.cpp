//
// Created by Tadeusz Kondracki on 4/2/22.
//

#include <string>
#include <cstdlib>
#include "Client.hpp"
#include "messages.hpp"

void Client::nick(std::vector<std::string> args) {
	if (args.size() != 1) {
		this->send_msg(ERR_NONICKNAMEGIVEN);
		return;
	}
	if (host.getClient(args[0])) {
		this->send_msg(ERR_NICKNAMEINUSE(args[0]));
		return;
	}
	if (!_user.nickname.empty()) {
		_queue.push_back(CH_NICK(_user.nickname, args[0]));
	}
	_user.nickname = args[0];
	if (!_user.username.empty() && !_user.wlc) {
		_queue.push_back(RPL_WELCOME(_user.nickname, _user.username));
		_user.wlc = true;
		_user.is_registered = true;
	}
}

void Client::userName(std::vector<std::string> args) {
	if (!_user.is_logged)
		return;
	if (_user.is_registered) {
		this->send_msg(ERR_ALREADYREGISTERED);
		return;
	}
	if (args.size() < 4) {
		this->send_msg(ERR_NEEDMOREPARAMS(this->getNickname(), "USER"));
		return;
	}
	_user.username = args[0];
	_user.mode = args[1];
	_user.unused = args[2];
	_user.real_name = args[3];
	if (!_user.nickname.empty() && !_user.wlc) {
		_queue.push_back(RPL_WELCOME(_user.nickname, _user.username));
		_user.wlc = true;
		_user.is_registered = true;
	}
}

void Client::join(std::vector<std::string> args) {
	std::string password;
	std::string channel_name;
	Channel *channel;

	if (args.empty()) {
		this->send_msg(ERR_NEEDMOREPARAMS(this->getNickname(), "JOIN"));
		return;
	}
	channel_name = args[0];
	channel = host.getChannels(channel_name);
	if (!channel) {
		channel = host.create_channel(channel_name, password, this);
	}
	if (channel->isInChannel(this))
		return;
	password = args.size() > 1 ? args[1] : "";
	if (channel->getPassword() != password) {
		this->send_msg(ERR_BADCHANNELKEY(args[0]));
		return;
	}
	if (channel->isMaxMode()) {
		if (channel->getMaxSize() <= channel->getClientsNumber()){
			channel->sendMessage(ERR_CHANNELISFULL(channel_name), this);
			return;
		}
	}
	this->joinChannel(channel);
}

void Client::quit() {
	_quit = true;

	if (this->getChannel() != NULL)
		this->leaveChannel();
	std::cout << "QUIT" << std::endl;
}

void Client::pass(std::vector<std::string> args) {
	if (_user.is_registered) {
		this->send_msg(ERR_ALREADYREGISTERED);
		return;
	}

	if (args.empty()) {
		this->send_msg(ERR_NEEDMOREPARAMS(this->getNickname(), "PASS"));
		return;
	}

	if (args[0].compare(host.getPass())) {
		this->send_msg(ERR_PASSWDMISMATCH);
		return;
	}

	if (!_user.nickname.empty() && !_user.username.empty() && !_user.wlc) {
		_queue.push_back(RPL_WELCOME(_user.nickname, _user.username));
		_user.wlc = true;
		_user.is_registered = true;
	}
	_user.is_logged = true;
}

void Client::list(Client *c) {
	host.listChannel(c);
}

void	Client::part(std::vector<std::string> args) {
	if (args.empty()) {
		this->send_msg(ERR_NEEDMOREPARAMS(this->getNickname(), "PART"));
		return;
	}
	std::string	channel_name = args[0];

	Channel	*channel = host.getChannels(channel_name);
	if (!channel) {
		this->send_msg(ERR_NOSUCHCHANNEL(this->getNickname(), channel_name));
		return;
	}
	if (!this->getChannel() || this->getChannel()->getName() != channel_name) {
		this->send_msg(ERR_NOTONCHANNEL(channel_name));
		return;
	}

	this->leaveChannel();
}

void Client::pmsg(std::vector<std::string> args) {
	std::string target;
	std::string message;
	Channel *channel;
	Client *dest;

	if (args.size() < 2 || args[0].empty()) {
		this->send_msg(ERR_NEEDMOREPARAMS(this->getNickname(), "PRIVMSG"));
		return;
	}
	for (std::vector<std::string>::iterator it = args.begin() + 1; it != args.end(); it++)
		message.append(*it + " ");
	target = args.at(0);
	if (target.at(0) == '#') {
		channel = host.getChannels(target);
		if (!channel) {
			this->send_msg(ERR_NOSUCHCHANNEL(this->getNickname(), target));
			return;
		}
		if (channel->isNMode()){
			if (!channel->isInChannel(this))
			{
				this->send_msg(ERR_CANNOTSENDTOCHAN(this->getNickname(), target));
				return;
			}
		}
		message = message.at(0) == ':' ? message.substr(1) : message;
		channel->sendMessage(CNF_PRIVMSG(this->getPrefix(), target, message), this);
		return;
	}

	dest = host.getClient(target);
	if (!dest) {
		this->send_msg(ERR_NOSUCHNICK(this->getNickname(), target));
		return;
	}
	message = message.at(0) == ':' ? message.substr(1) : message;
	dest->send_msg(CNF_PRIVMSG(this->getPrefix(), target, message));
}

void	Client::ping(std::vector<std::string> args)
{
	if (args.empty()){
		this->send_msg(ERR_NOORIGIN);
		return;
	}
	if (args[0] != this->host.getAddress()){
		this->send_msg(ERR_NOSUCHSERVER(args[0]));
		return;
	}
	this->send_msg("PONG " + this->host.getAddress() + "\r\n");
}

void	Client::pong(std::vector<std::string> args)
{
	if (args.empty()){
		this->send_msg(ERR_NOORIGIN);
		return;
	}
	this->set_last_activity(std::time(NULL));
}

void	Client::kick(std::vector<std::string> args)
{
	std::string	channel_name;
	Channel *channel;
	Client *target;

	if (args.size() < 2) {
		this->send_msg(ERR_NEEDMOREPARAMS(this->getNickname(), "KICK"));
		return;
	}
	channel_name = args[0];
	channel = host.getChannels(channel_name);
	if (!channel) {
		this->send_msg(ERR_NOSUCHCHANNEL(this->getNickname(), channel_name));
		return;
	}
	if (!channel->isAdmin(this) && !channel->isOperator(this))
	{
		this->send_msg(ERR_CHANOPRIVSNEEDED(this->getNickname(), channel_name));
		return;
	}
	target = channel->getClient(args[1]);
	if (!target) {
		this->send_msg(ERR_USERNOTINCHANNEL(args[1], channel_name));
		return;
	}
	if (!channel->getClient(this->getUsername())) {
		this->send_msg(ERR_NOTONCHANNEL(channel_name));
		return;
	}
	channel->sendMessage(KICK_MSG(args[1], channel_name), this);
	channel->removeUser(target);
}

//TODO -> Reformat this code and make tests
void Client::mode(std::vector<std::string> args)
{
	std::string	target;
	Channel		*channel;
	int			i = 0;
	char		modeChar;

	if (args.size() < 2) {
		this->send_msg(ERR_NEEDMOREPARAMS(this->getNickname(), "MODE"));
		return;
	}
	if ((*(args[0].begin()) != '#' && *(args[0].begin()) != '$'))
		return;
	target = args.at(0);
	channel = host.getChannels(target);
	if (!channel) {
		this->send_msg(ERR_NOSUCHCHANNEL(this->getNickname(), target));
		return;
	}
	if (!channel->isAdmin(this) && !channel->isOperator(this)) {
		this->send_msg(ERR_CHANOPRIVSNEEDED(this->getNickname(), target));
		return;
	}

	modeChar = args[1][i];
	while (modeChar) {
		char prevC;
		if (i > 0)
			prevC = args[1][i - 1];
		else
			prevC = '\0';
		bool active = prevC == '+';
		switch (modeChar) {

			case 'o': {
				Client	*targetUser;
				targetUser = channel->getClient(args[2]);
				if (!targetUser)
				{
					this->send_msg(ERR_NOSUCHNICK(args[2], target));
					return;
				}
				if (active)
					channel->setOperator(targetUser);
				else
					channel->removeOperator(targetUser);
				channel->sendMessage(RPL_MODE(this->getPrefix(), channel->getName(), (active ? "+o" : "-o"), ""), this);
				break;
			}
			case 'n': {
				channel->setNMode(active);
				channel->sendMessage(RPL_MODE(this->getPrefix(), channel->getName(), (active ? "+n" : "-n"), ""), this);
				break;
			}
			case 'l': {
				int	size;
				if (active && args.size() == 3)
					size = atoi(args[2].c_str());
				else
					size = 0;
				channel->setMaxSize(active, size);
				channel->sendMessage(RPL_MODE(this->getPrefix(), channel->getName(), (active ? "+l" : "-l"), ""), this);
				break;
			}
			default:
				break;
		}
		i++;
		modeChar = args[1][i];
	}
}

void	Client::notice(std::vector<std::string> args) {
	std::string	target;
	std::string	message;
	Channel *channel;
	Client *dest;

	if (args.size() < 2)
		return;
	for (std::vector<std::string>::iterator it = args.begin() + 1; it != args.end(); it++)
		message.append(*it + " ");
	message = message.at(0) == ':' ? message.substr(1) : message;
	target = args.at(0);
	if (target.at(0) == '#') {
		channel = host.getChannels(target);
		if (!channel)
			return;
		if (channel->isNMode())
			if (!channel->isInChannel(this))
				return;
		message = message.at(0) == ':' ? message.substr(1) : message;
		channel->sendMessage(CNF_PRIVMSG(this->getPrefix(), target, message), this);
		return;
	}
	dest = host.getClient(target);
	if (!dest)
		return;
	message = message.at(0) == ':' ? message.substr(1) : message;
	dest->send_msg(CNF_PRIVMSG(this->getPrefix(), target, message));
}
