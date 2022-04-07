//
// Created by Tadeusz Kondracki on 4/7/22.
//

#ifndef IRCSERV_TESTMANAGER_HPP
#define IRCSERV_TESTMANAGER_HPP

#include "Client.hpp"
#include "Server.hpp"

typedef std::vector<std::string> &q_ref;

class TestManager
{
public:

	Server server;
	Client client;

	q_ref &test_cmd(std::string cmd);

	TestManager();
	~TestManager();
};


#endif //IRCSERV_TESTMANAGER_HPP
