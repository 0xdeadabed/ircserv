//
// Created by Hajar Sabir on 6/9/22.
//


#include "bot.hpp"

int	main(int argc, char **argv) {
	std::string pass;

	try {

		if (argc < 3 || argc > 4) {
			throw std::runtime_error("Usage: ./ircbot <host> <port> <password>");
		}

		Bot bot(argv[1], std::stoi(argv[2]), argv[3]);
		bot.run();
		return 0;
	}
	catch (const std::exception &ex) {
		std::cerr << ex.what() << std::endl;
	}
}
