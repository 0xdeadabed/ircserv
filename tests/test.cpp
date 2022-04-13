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
	manager->test_cmd("PASS 1234\r\n");
	q_ref queue = manager->get_queue();
	cr_assert(!queue.empty(), "Connection: no answer");
	cr_expect(queue.at(0) == "001 * :Welcome to the Internet Relay Network johnny!John@42.lausanne.ch\r\n", "Connection: answer contents");
	cr_expect(queue.size() == 1, "Connection: too many answers");
	manager->clear_queue();
}

Test(cmds, nick_ERR_NONICKNAMEGIVEN) {
	manager->test_cmd("NICK\r\n");
	q_ref queue = manager->get_queue();
	cr_assert(!queue.empty(), "ERR_NONICKNAMEGIVEN: no answer");
	cr_expect(queue.at(0) == "431 :No nickname given\r\n", "ERR_NONICKNAMEGIVEN: answer contents");
	cr_expect(queue.size() == 1, "ERR_NONICKNAMEGIVEN: too many answers");
	manager->clear_queue();
}

Test(cmds, ERR_NEEDMOREPARAMS) {
	q_ref queue = manager->get_queue();
	//USER
	manager->test_cmd("USER a b c\r\n");
	cr_assert(!queue.empty(), "ERR_NEEDMOREPARAMS: USER: no answer");
	cr_expect(queue.at(0) == "461 USER :Not enough parameters\r\n", "ERR_NEEDMOREPARAMS: USER: answer contents");
	cr_expect(queue.size() == 1, "ERR_NEEDMOREPARAMS: USER: too many answers");
	manager->clear_queue();
	//PASS
	manager->test_cmd("PASS\r\n");
	cr_assert(!queue.empty(), "ERR_NEEDMOREPARAMS: PASS: no answer");
	cr_expect(queue.at(0) == "461 PASS :Not enough parameters\r\n", "ERR_NEEDMOREPARAMS: PASS: answer contents");
	cr_expect(queue.size() == 1, "ERR_NEEDMOREPARAMS: PASS: too many answers");
	manager->clear_queue();
}

Test(cmds, ERR_ALREADYREGISTERED) {
	q_ref queue = manager->get_queue();
	//registering
	manager->test_cmd("NICK johnny\r\n");
	manager->test_cmd("USER John 0 * :John Smith\r\n");
	manager->test_cmd("PASS 1234\r\n");
	manager->clear_queue();
	//PASS
	manager->test_cmd("PASS 1234\r\n");
	cr_assert(!queue.empty(), "ERR_ALREADYREGISTERED: PASS: no answer");
	cr_expect(queue.at(0) == "462 :Unauthorized command (already registered)\r\n", "ERR_ALREADYREGISTERED: PASS: answer contents");
	cr_expect(queue.size() == 1, "ERR_ALREADYREGISTERED: PASS: too many answers");
	manager->clear_queue();
	//USER
	manager->test_cmd("USER John 0 * :John Smith\r\n");
	cr_assert(!queue.empty(), "ERR_ALREADYREGISTERED: USER: no answer");
	cr_expect(queue.at(0) == "462 :Unauthorized command (already registered)\r\n", "ERR_ALREADYREGISTERED: USER: answer contents");
	cr_expect(queue.size() == 1, "ERR_ALREADYREGISTERED: USER: too many answers");
	manager->clear_queue();
}
