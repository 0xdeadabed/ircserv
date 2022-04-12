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

void	TestManager::test_cmd(const std::string& cmd){
	client.manage_command(cmd);
}

q_ref TestManager::get_queue(){
	return client._queue;
}

void TestManager::print_queue(q_ref q)
{
	std::cout << "queue dump:"<< std::endl;
	for (int i = 0; i < (int) q.size(); ++i)
	{
		std::cout << q.at(i);
	}
	std::cout << "---" << std::endl;
}

void TestManager::clear_queue()
{
	client._queue.clear();
}
