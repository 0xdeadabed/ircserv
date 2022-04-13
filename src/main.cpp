//
// Created by tadeu on 3/16/2022.
//

#include "Server.hpp"
#include <cstdlib>

int main(int argc, char **argv){

#if defined(IRC_LOG) or defined(IRC_LOG_FILE)
	std::ofstream log;
	log.open("irc_log", std::ios::out | std::ios::trunc);
	log.close();
#endif
	if (argc != 3){
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	Server ircserv(atoi(argv[1]), argv[2]);
	ircserv.loop();
	
	return 0;
}

