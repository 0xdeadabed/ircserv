//
// Created by Tadeusz Kondracki on 3/29/22.
//

#include "Client.hpp"

Client::Client()
{
	//default values
}

Client::Client(Client const &inst)
{
	*this = inst;
}

Client::~Client()
{

}

Client &Client::operator=(Client const &rhs)
{
	this.
}

std::iostream &operator<<(std::iostream &io, Client const & client){
	io << "client todo?";
	return io;
}
