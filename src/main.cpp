//
// Created by tadeu on 3/16/2022.
//

#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>

#define SERVER_PORT 4242
#define MAXLEN 1024

int err(const std::string& str);

int main(int argc, char** argv){
	int					listen_fd, connect_fd, n;
	struct sockaddr_in	server_address;
	char				receive_buff[MAXLEN];

	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd < 0)
		return err("socket not created");

	bzero(&server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(SERVER_PORT);

	if (bind(listen_fd, (sockaddr *) &server_address, sizeof(server_address)) < 0)
		return err("socket not bound");
	if (listen(listen_fd, 10) < 0)
		return err("socket not listening");

	while (true) {
		struct sockaddr_in	addr;
		socklen_t			adrr_len;\
		char				client_addr[MAXLEN + 1];

		connect_fd = accept(listen_fd, (sockaddr *) &addr, &adrr_len);
		inet_ntop(AF_INET, &addr, client_addr, MAXLEN);
		std::cout << "incoming connection:\nfrom: " << client_addr << "\nmessage:" << std::endl;

		bzero(receive_buff, MAXLEN);
		while ((n = read(connect_fd, receive_buff, MAXLEN - 1)) > 0){
			std::cout << receive_buff << std::endl;
			if (receive_buff[n-1] == '\n')
				break;
			bzero(receive_buff, MAXLEN);
		}
		if (n < 0)
			return err("read_err");
		std::cout << "---" << std::endl;

		//todo errors
		write(connect_fd, "bye", 3);
		close(connect_fd);
	}
}

int err(const std::string& str){
	std::cerr << str << std::endl;
	return 1;
}
