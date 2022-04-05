/**
 * @author   : Hajar Sabir (wubbalubba@dub.dub)
 * @created  : 4/2/22.
 */

#ifndef IRCSERV_MESSAGES_HPP
#define IRCSERV_MESSAGES_HPP

#define RPL_JOIN(client, channel)		":" + client + " JOINED " + channel + "\n"
#define ERR_NONNICK						"431 * :Nickname not given\n"
//#define RPL_WELCOME(nick, user, host)	"Welcome to the Internet Relay Network " + nick + !user + @host
#define ERR_NICKNAMEINUSE				"433 * :Nickname is already in use\n"
#define RPL_WELCOME(nick, user, host)			"001 * :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + "42.lausanne.ch\n"

#endif //IRCSERV_MESSAGES_HPP
