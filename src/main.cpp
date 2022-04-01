//
// Created by tadeu on 3/16/2022.
//

#include "Server.hpp"

int main(int argc, char **argv){
	
	if (argc != 3)
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
	Server ircserv(argv[1], argv[2]);
	ircserv.loop();
	
	return 0;
}

