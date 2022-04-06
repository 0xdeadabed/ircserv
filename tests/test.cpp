// tests.cpp template file

#include <unistd.h>
#include "Client.hpp"
#include "criterion/criterion.h"

Test(example_tests, success) {
	cr_expect(true, "this test should be true");
}

//Setup adn teardown
Server *serv = NULL;
Client *test = NULL;
int		p[2];

void suite_setup(){
	serv = new Server();
	pipe(p);
	test = new Client(*serv, p[0]);
}

void suite_teardown(){
	delete serv;
	close(p[1]);
}

TestSuite(parsing, .init=suite_setup, .fini=suite_teardown);

Test(parsing, valid) {
	try
	{
		write(p[1], "NICK John\n", strlen("NICK John\n"));
		test->manage_events(POLLIN);
		std::vector<std::string> &queue = test->get_queue();
		cr_expect("001 * :Welcome to the Internet Relay Network John!John@42.lausanne.ch\n" == queue[0]);
	}
	catch (std::exception &e)
	{
		std::cout << " ERNNO = " << strerror(errno) <<  std::endl;
	}
}
