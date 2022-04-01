//
// Created by Tadeusz Kondracki on 4/1/22.
//

#ifndef IRCSERV_IRC_CMD_H
# define IRCSERV_IRC_CMD_H

struct s_irc_command {
	std::string					origin;
	std::string					cmd;
	std::vector<std::string>	args;
} typedef	irc_cmd;

#endif //IRCSERV_IRC_CMD_H
