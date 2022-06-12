//
// Created by Hajar Sabir on 6/9/22.
//

#include "bot.hpp"
#include <arpa/inet.h>
#include <sys/wait.h>
#include <unistd.h>
#include <csignal>
#include <cstring>
#include <cstdlib>
#include <cstdio>


bool running = true;

static void destroy(int n) {
	(void) n;
	running = false;
}

Bot::Bot(const std::string &host, const int &port, const std::string &password, const std::string channel) :
	 _pass(password), _chan(channel), _server_address()
	{
		_host = host;
		_port = port;
		memset(&_server_address, '\0', sizeof(_server_address));
		if ((_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			throw std::runtime_error("[-] Marvin: failed to create socket\n");
		_server_address.sin_family = AF_INET;
		_server_address.sin_addr.s_addr = inet_addr(_host.c_str());
		_server_address.sin_port = htons(_port);
		if (connect(_sock, (sockaddr *) &_server_address, sizeof(_server_address)) < 0)
			throw std::runtime_error("[-] Marvin: failed to connect to the remote host\n");
	}

Bot::~Bot() {
	send_cmd("QUIT");
	close(this->_sock);
	std::cout << "[x] Marvin: quit" << std::endl;
}

void	Bot::send_cmd(std::string arg) const {
	arg.append("\r\n");
	send(_sock, arg.c_str(), arg.size(), 0);
}

void	Bot::replay_cmd(const std::string &src, std::string &cmd, std::vector<std::string> arg) {
	std::string				nick = src;
	std::string::size_type	pos = nick.find('!');

	if (pos != std::string::npos)
		nick = nick.substr(1, pos - 1);
	if (cmd == "PRIVMSG") {
		if (this->args[2][0] == '#')
			nick = this->args[2];
		//TODO -> a better method to parse and respond
		if (args.size() >= 2) {
			if (arg[2] == "Marvin")
				send_cmd("PRIVMSG " + nick + " : Hello, I'm marvin");
			else if (arg[2] == "windows")
				send_cmd("PRIVMSG " + nick + " : Don't use cursed word");
			else if (arg[2] == "DCC")
				dcc_send(arg);
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
			throw std::runtime_error("[-] Marvin: failed to recv from socket\n");
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

	std::cout << "[O] Marvin: joined an irc server at " << _host << std::endl;
	std::cout << "[+] NICK: Marvin" << std::endl;
	std::cout << "[+] JOIN: " << _chan << std::endl;

	e_listener();
}

void write_file(int sockfd, const std::string& filename){
	ssize_t	n;
	FILE	*fp;
	char	buffer[1024];

	fp = fopen(filename.c_str(), "w");
	while (1) {
		n = recv(sockfd, buffer, 1024, 0);
		if (n <= 0){
			break;
			return;
		}
		std::cout << "[+] Marvin: received packages" << std::endl;
		std::cout << "[+] Marvin: writing to file..." << std::endl;
		//fprintf(fp, "%s", buffer);
		fwrite(buffer, 1, n, fp);
		bzero(buffer, 1024);
		//std::cout << "DEBUG" << std::endl;
	}
}

void	Bot::server_listen(std::vector<std::string> arg) {
	int	e;

	int sock_fd, new_sock;
	struct sockaddr_in serverAddr = {};
	struct sockaddr_in newAddr = {};
	socklen_t socklen;

	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_fd < 0)
		throw std::runtime_error("[-] Marvin: failed to create a socket");
	std::cout << "[+] Marvin: created a socket successfully.\n";

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = 8080;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	e = bind(sock_fd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(e < 0)
		throw std::runtime_error("[-] Marvin: failed to bind");
	if(listen(sock_fd, 10) == 0){
		std::cout << "[+] Marvin: the server is listening...." << std::endl;
	}else{
		throw std::runtime_error("[-] Marvin: failed to listen");
	}

	socklen = sizeof(newAddr);
	new_sock = accept(sock_fd, (struct sockaddr*)&newAddr, &socklen);
	std::cout << "[+] Marvin: accepted connection" << std::endl;
	write_file(new_sock, arg[4]);
	close(sock_fd);
	std::cout << "[+] Marvin: the file has been writen successfully..." << std::endl;
	std::cout << "[+] Marvin: server listening stops..." << std::endl;
	exit(0);
}

void send_file(FILE *fp, int sockfd){
	char data[1024] = {0};

	while(fgets(data, 1024, fp) != NULL) {
		if (send(sockfd, data, sizeof(data), 0) == -1)
			throw std::runtime_error("[-] Marvin: failed to send a file");
		bzero(data, 1024);
	}
}

void	Bot::client_send(std::vector<std::string> arg) {
	int		sock_fd;
	int		e;
	FILE	*fp;

	struct sockaddr_in serverAddr = {};

	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_fd < 0)
		throw std::runtime_error("[-] Marvin: failed to create a socket [CLIENT]");

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = 8080;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	e = connect(sock_fd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(e == -1)
		throw std::runtime_error("[-] Marvin: failed to connect to a socket [CLIENT]");

	fp = fopen(arg[3].c_str(), "r");
	if (fp == NULL)
		throw std::runtime_error("[-] Marvin: failed to read the file [CLIENT]");

	send_file(fp, sock_fd);
	std::cout << "[+] Marvin: sent the file successfully\n";

	close(sock_fd);
}

void Bot::dcc_send(const std::vector<std::string>& arg) {
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		throw std::runtime_error("[-] Marvin: failed to fork a process");
	if (pid == 0)
		server_listen(arg);
	usleep(500);
	client_send(arg);
}
