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

q_ref TestManager::test_cmd(std::string cmd){
	client.manage_command(cmd);
	return client._queue;
}
