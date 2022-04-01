//
// Created by tadeu on 3/16/2022.
//

#include "Server.hpp"

int main(){
	Server ircserv(6697, "password");

	ircserv.loop();
}

