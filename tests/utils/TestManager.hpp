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

	void	test_cmd(const std::string& cmd);
	q_ref	get_queue();
	void	clear_queue();
	static void	print_queue(q_ref q);

	TestManager();
	~TestManager();
};


#endif //IRCSERV_TESTMANAGER_HPP
