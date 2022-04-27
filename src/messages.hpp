/**
 * @author   : Hajar Sabir (wubbalubba@dub.dub)
 * @created  : 4/2/22.
 */

#ifndef IRCSERV_MESSAGES_HPP
#define IRCSERV_MESSAGES_HPP

#define RPL_JOIN(client, channel)				":" + client + " JOINED " + channel + "\r\n"
#define ERR_NONICKNAMEGIVEN						"431 :No nickname given\r\n"
#define ERR_NICKNAMEINUSE(nick)					"433 * " + nick + " :Nickname is already in use\r\n"
#define RPL_WELCOME(nick, user)					"001 * :Welcome to the Internet Relay Network " + nick + "!" + user + "@42.lausanne.ch\r\n"
#define ERR_ALREADYREGISTERED					"462 :Unauthorized command (already registered)\r\n"
#define	ERR_NEEDMOREPARAMS(cmd)					"461 " cmd " :Not enough parameters\r\n"
#define	ERR_PASSWDMISMATCH						"464 * :Password incorrect\r\n"
#define	CH_NICK(o_nick, n_nick)					o_nick + " changed his nickname to " + n_nick + "\r\n"
#define ERR_BADCHANNELKEY(channel)				"475 " + channel + " :Cannot join channel (+k)\r\n"
#define RPL_NAMREPLY(client, channels, admin)	"353 " + client + " + " + channels + "@" + admin + "\r\n"
#define RPL_ENDOFNAMES(client, channel)			"366 " + client + " " + channel + " :End of /NAMES list.\r\n"
#define ERR_NOSUCHCHANNEL(channel)				"403 " + channel + " :No such channel\r\n"
#define ERR_NOTONCHANNEL(channel)				"442 " + channel + " :You're not on that channel \n"
#define ERR_UNKNOWNCOMMAND(command)				"421 " + command + " :Unknown command\r\n"
#define ERR_NOORIGIN							"409 :No origin specified\r\n"
#define ERR_NOSUCHSERVER(serv_addr)				"402 " + serv_addr + " :No such server\r\n"


#endif //IRCSERV_MESSAGES_HPP
