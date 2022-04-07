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
	try
	{
		q_ref queue = manager->test_cmd("USER John");
		cr_assert(!queue.empty(), "NICK: no answer");
		cr_expect(queue.at(0) == "smth", "NICK: wrong answer");
		cr_expect(queue.size() > 1, "NICK: too many answers");
	}
	catch (std::exception &e)
	{
		std::cout << "caught smth " << e.what() <<  std::endl;
	}
}
