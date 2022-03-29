//
// Created by Tadeusz Kondracki on 3/29/22.
//

#ifndef FT_IRC_SERVER_HPP
#define FT_IRC_SERVER_HPP

#include <iostream>

class Server
{
public:
	Server();
	Server(Server const &inst);
	~Server();

	Server &operator=(Server const &rhs);
};

std::iostream &operator<<(const Server & serv){

}

#endif //FT_IRC_SERVER_HPP
