// tests.cpp template file

#include <unistd.h>
#include "utils/TestManager.hpp"
#include "criterion/criterion.h"

Test(example_tests, success) {
	cr_expect(true, "this test should be true");
}

//Setup adn teardown
TestManager *manager;

void suite_setup(){
	manager = new TestManager();
}

void suite_teardown(){
	delete manager;
}

TestSuite(nick, .init=suite_setup, .fini=suite_teardown);

Test(nick, valid) {
	q_point queue = manager->test_cmd("NICK John");
	cr_assert(!queue->empty(), "NICK: no answer");
	cr_expect(queue->at(0) == "001 * :Welcome to the Internet Relay Network John!John@42.lausanne.ch\r\n", "NICK: wrong answer");
	cr_expect(queue->size() == 1, "NICK: too many answers");
	delete queue;
}

Test(nick, invalid) {
	q_point queue = manager->test_cmd("NICK");
	cr_assert(!queue->empty(), "NICK: no answer");
	std::cout << queue->at(0) << std::endl;
	cr_expect(queue->at(0) == "431 :No nickname given\r\n", "NICK: wrong answer");
	cr_expect(queue->size() == 1, "NICK: too many answers");
	delete queue;
}
