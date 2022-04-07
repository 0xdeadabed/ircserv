//
// Created by Tadeusz Kondracki on 4/7/22.
//

#include "TestManager.hpp"

TestManager::TestManager():
	server(),
	client(Client(server, -1))
{
	//default values
}

TestManager::~TestManager()
{

}

q_point TestManager::test_cmd(std::string cmd){
	q_point ret;

	client.manage_command(cmd);
	ret = new std::vector<std::string>(client._queue);
	client._queue.clear();
	return ret;
}
