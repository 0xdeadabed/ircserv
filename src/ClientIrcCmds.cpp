//
// Created by Tadeusz Kondracki on 4/2/22.
//

#include "Client.hpp"

void	Client::nick(){
	std::cout << "got nick command" << std::endl;
}

void	Client::user(){
	std::cout << "got user command" << std::endl;
}

void	Client::join(){
	std::cout << "got join command" << std::endl;
}

void	Client::quit(){
	std::cout << "got quit command" << std::endl;
}

void	Client::pass(){
	std::cout << "got pass command" << std::endl;
}
