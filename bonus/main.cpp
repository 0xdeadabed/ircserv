//
// Created by Hajar Sabir on 6/9/22.
//


#include "bot.hpp"

int	main(int argc, char **argv) {
	std::string pass;
	std::string channel;

	try {

		if (argc < 3 || argc > 5) {
			throw std::runtime_error("Usage: ./ircbot <host> <port> <password> [channel]");
		}
		argc == 5 ? channel = argv[4] : channel = "#Marvin";
		Bot bot(argv[1], atoi(argv[2]), argv[3], channel);
		bot.run();
		return 0;
	}
	catch (const std::exception &ex) {
		std::cerr << ex.what() << std::endl;
	}
}
