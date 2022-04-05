/**
 * @author   : Hajar Sabir (wubbalubba@dub.dub)
 * @created  : 4/2/22.
 */

#ifndef IRCSERV_MESSAGES_HPP
#define IRCSERV_MESSAGES_HPP

#define RPL_JOIN(client, channel)		":" + client + " JOINED " + channel + "\n"
#define ERR_NONNICK						"431 * :Nickname not given"
//#define RPL_WELCOME(nick, user, host)	"Welcome to the Internet Relay Network " + nick + !user + @host
#define ERR_USEDNICK					"433 * :Nickname is already in use"
#define RPL_WELCOME(nick)				"001 * :Welcome to the Internet Relay Network network " + nick

#endif //IRCSERV_MESSAGES_HPP
