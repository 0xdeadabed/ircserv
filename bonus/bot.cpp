//
// Created by Hajar Sabir on 6/9/22.
//

#include "bot.hpp"
#include <arpa/inet.h>
#include <sys/wait.h>
#include <unistd.h>
#include <poll.h>

int	quit = 0;

Bot::Bot(const std::string &host, const int &port, const std::string &password) :
	_host(host), _port(port), _pass(password)
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
	std::cout << "Marvin: died\n";
}

void	Bot::send_cmd(std::string args) {
	args.append("\r\n");
	send(_sock, args.c_str(), args.size(), 0);
}

void	Bot::e_listener() {
	char 			buffer[256];
	int 			length;

	send_cmd("JOIN #ft");
	send_cmd("PRIVMSG #ft Hello world");
	while (quit == 0)
		usleep(1);
//	close(this->_sock);
}

void	Bot::run() {
	pid_t 		c_pid;
	std::string input;

	// Init the bot
	send_cmd("PASS " + _pass);
	send_cmd("NICK Marvin");
	send_cmd("USER marvin marvin 127.0.0.1 :Chad Marvin");

	if ((c_pid = fork()) < 0)
		std::runtime_error("Marvin: failed to fork a process\n");
	if (c_pid == 0)
		e_listener();
	getline(std::cin, input);
	while (input != "QUIT") {
		waitpid(c_pid, 0, 0);
		getline(std::cin, input);
		usleep(500);
	}
	quit = 1;
	send_cmd("QUIT");
	close(this->_sock);
}