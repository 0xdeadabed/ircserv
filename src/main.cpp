//
// Created by tadeu on 3/16/2022.
//

#include "Server.hpp"
#include <stdlib.h>

int main(int argc, char **argv){
	
	if (argc != 3){
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	Server ircserv(atoi(argv[1]), argv[2]);
	ircserv.loop();
	
	return 0;
}

