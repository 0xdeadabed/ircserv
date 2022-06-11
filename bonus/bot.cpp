//
// Created by Hajar Sabir on 6/9/22.
//

#include "bot.hpp"
#include <arpa/inet.h>
#include <sys/wait.h>
#include <unistd.h>
#include <csignal>
#include <thread>

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
			throw std::runtime_error("Marvin: failed to recv from socket\n");
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

void Bot::dcc_send(std::vector<std::string> arg) {
	char	buffer[1024];
	char	sendbuffer[100];
	int 	sfd =0;
	int		n=0;
	int		b;
	FILE	*fpr;
	FILE	*fpw;
	pid_t	pid;

	fpw = fopen(arg[4].c_str(), "wb");
	pid = fork();
	if (pid < 0)
		throw std::runtime_error("[-] Marvin: failed to fork a process");
	if (pid == 0)
		recv_f(fpw);
	struct sockaddr_in serv_addr;
	memset(buffer, '0', sizeof(buffer));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(6697);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	sfd = socket(AF_INET, SOCK_STREAM, 0);

	b=connect(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if (b==-1) {
		throw std::runtime_error("[-] Marvin: failed to connect");
	}

	fpr = fopen(arg[3].c_str(), "rb");
	if (!fpr) {
		throw std::runtime_error("[-] Marvin: failed to open a file");
	}
	while (fread(&sendbuffer, 1, 1024, fpr) > 0) {
		send(sfd, sendbuffer, b, 0);
	}
	fclose(fpr);

}

void	Bot::recv_f(FILE *fpw) {
	int		fd =0;
	int		confd = 0;
	int		b;
	int		tot;
	int		num;

	char	buffer[1024];

	struct sockaddr_in serv_addr;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
		throw std::runtime_error("[-] Marvin: failed to open a socket");

	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(buffer, '0', sizeof(buffer));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(6696);
	int serv_address_len = sizeof(serv_addr);

	bind(fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	listen(fd, 10);

	while (1) {
		confd = accept(fd, (struct sockaddr *) &serv_addr, (socklen_t *) &serv_address_len);
		if (confd < 0) {
			throw std::runtime_error("[-] Marvin: failed to accept a socket");
			continue;
		}

		tot=0;
		if (fpw != NULL) {
			while ((b = recv(confd, buffer, 1024, 0)) > 0) {
				tot += b;
				fwrite(buffer, 1, b, fpw);
			}
			if (b < 0)
				throw std::runtime_error("[-] Marvin: failed to write");
			fclose(fpw);
		} else {
			throw std::runtime_error("[-] Marvin: failed to open the output file");
		}
		close(confd);
	}
}