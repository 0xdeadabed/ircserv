// tests.cpp template file

#include <unistd.h>
#include "utils/TestManager.hpp"
#include "criterion/criterion.h"

//Setup adn teardown
TestManager *manager;

void suite_setup(){
	manager = new TestManager();
}

void suite_teardown(){
	delete manager;
}

TestSuite(cmds, .init=suite_setup, .fini=suite_teardown);

Test(cmds, connection) {
	manager->test_cmd("NICK johnny\r\n");
	manager->test_cmd("USER John 0 * :John Smith\r\n");
	manager->test_cmd("PASS password\r\n");
	q_ref queue = manager->get_queue();
	cr_assert(!queue.empty(), "no answer");
	cr_expect(queue.at(0) == "001 * :Welcome to the Internet Relay Network johnny!John@42.lausanne.ch\r\n", "wrong answer");
	cr_expect(queue.size() == 1, "too many answers");
	manager->clear_queue();
}

Test(cmds, nick_ERR_NONICKNAMEGIVEN) {
	manager->test_cmd("NICK\r\n");
	q_ref queue = manager->get_queue();
	cr_assert(!queue.empty(), "no answer");
	cr_expect(queue.at(0) == "431 :No nickname given\r\n", "should answer no nickname given");
	cr_expect(queue.size() == 1, "too many answers");
	manager->clear_queue();
}

Test(cmds, ERR_NEEDMOREPARAMS) {
	q_ref queue = manager->get_queue();
	//USER
	manager->test_cmd("USER a b c\r\n");
	cr_assert(!queue.empty(), "no answer");
	cr_expect(queue.at(0) == "461 USER :Not enough parameters\r\n", "should answer not enough params");
	cr_expect(queue.size() == 1, "too many answers");
	manager->clear_queue();
	//PASS
	manager->test_cmd("PASS\r\n");
	cr_assert(!queue.empty(), "no answer");
	cr_expect(queue.at(0) == "461 PASS :Not enough parameters\r\n", "should answer not enough params");
	cr_expect(queue.size() == 1, "too many answers");
	manager->clear_queue();
}

Test(cmds, ERR_ALREADYREGISTERED) {
	q_ref queue = manager->get_queue();
	//registering
	manager->test_cmd("NICK johnny\r\n");
	manager->test_cmd("USER John 0 * :John Smith\r\n");
	manager->test_cmd("PASS password\r\n");
	manager->clear_queue();
	//PASS
	manager->test_cmd("PASS pass\r\n");
	cr_assert(!queue.empty(), "no answer");
	cr_expect(queue.at(0) == "462 :Unauthorized command (already registered)\r\n", "should answer already registered");
	cr_expect(queue.size() == 1, "too many answers");
	manager->clear_queue();
	//USER
	manager->test_cmd("USER John 0 * :John Smith\r\n");
	cr_assert(!queue.empty(), "no answer");
	cr_expect(queue.at(0) == "462 :Unauthorized command (already registered)\r\n", "should answer already registered");
	cr_expect(queue.size() == 1, "too many answers");
	manager->clear_queue();
}
