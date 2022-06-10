//
// Created by Hajar Sabir on 6/9/22.
//

#include "bot.hpp"
#include <arpa/inet.h>
#include <sys/wait.h>
#include <unistd.h>
#include <csignal>

bool running = true;

static void destroy(int n) {
	(void) n;
	running = false;
}

Bot::Bot(const std::string &host, const int &port, const std::string &password, const std::string channel) :
	_host(host), _port(port), _pass(password), _chan(channel), _server_address()
	{
		memset(&_server_address, '\0', sizeof(_server_address));
		if ((_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			std::runtime_error("Marvin: failed to create socket\n");
		_server_address.sin_family = AF_INET;
		_server_address.sin_addr.s_addr = inet_addr(_host.c_str());
		_server_address.sin_port = htons(_port);
		if (connect(_sock, (sockaddr *) &_server_address, sizeof(_server_address)) < 0)
			std::runtime_error("Marvin: failed to connect to the remote host\n");
	}

Bot::~Bot() {
	send_cmd("QUIT");
	close(this->_sock);
}

void	Bot::send_cmd(std::string args) const {
	args.append("\r\n");
	send(_sock, args.c_str(), args.size(), 0);
}

void	Bot::replay_cmd(const std::string &src, const std::string &cmd, std::vector<std::string> args) const {
	std::string				nick = src;
	std::string::size_type	pos = nick.find('!');

	if (pos != std::string::npos)
		nick = nick.substr(1, pos - 1);
	if (cmd == "PRIVMSG") {
		if (this->args[2][0] == '#')
			nick = this->args[2];
		//TODO -> a better method to parse and respond
		if (args.size() >= 2) {
			if (args.at(1).substr(1) == "Marvin")
				send_cmd("PRIVMSG " + nick + " : Hello, I'm marvin");
			if (args.at(1).substr(1) == "windows")
				send_cmd("PRIVMSG " + nick + " : Don't use cursed word");
			return;
		}
		send_cmd("PRIVMSG " + nick + " : Hello, I'm marvin");
		return;
	}
}

void	Bot::split(std::string &buffer) {
	size_t next;

	while ((next = buffer.find(' ')) != std::string::npos) {
		this->args.push_back(buffer.substr(0, next));
		buffer.erase(0, next + 1);
	}
	this->args.push_back(buffer);
}

void	Bot::read_msg(const std::string &message) {
	std::string	line = message;

	this->args.erase(this->args.begin(), this->args.end());
	split(line);
	if (this->args.size() < 2)
		return;
	std::string	src = args.at(0);
	std::string	cmd = args.at(1);

	if (atoi(cmd.c_str()) > 0)
		return;
	replay_cmd(src, cmd,std::vector<std::string>(args.begin() + 2, args.end()));
}

void	Bot::e_listener() {
	signal(SIGINT, destroy);
	char 			buffer[1024];
	ssize_t			len;

	while (1)
	{
		if (!running)
			break;
		len = recv(this->_sock, buffer, 1024, 0);
		if (len < 0)
			std::runtime_error("Marvin: failed to recv from socket\n");
		buffer[len] = 0;
		this->read_msg(std::string(buffer, len));
		memset(buffer, 0, sizeof buffer);
	}
}

void	Bot::run() {
	// Init the bot
	send_cmd("PASS " + _pass);
	send_cmd("NICK Marvin");
	send_cmd("USER marvin marvin " + _host + " :Chad Marvin");
	send_cmd("JOIN " + _chan);

	e_listener();
}