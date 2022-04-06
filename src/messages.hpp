/**
 * @author   : Hajar Sabir (wubbalubba@dub.dub)
 * @created  : 4/2/22.
 */

#ifndef IRCSERV_MESSAGES_HPP
#define IRCSERV_MESSAGES_HPP

#define RPL_JOIN(client, channel)				":" + client + " JOINED " + channel + "\n"
#define ERR_NONNICK								"431 * :Nickname not given \n"
#define ERR_NICKNAMEINUSE						"433 * :Nickname is already in use \n"
#define RPL_WELCOME(nick, user, host)			"001 * :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + "42.lausanne.ch \n"
#define ERR_ALREADYREGISTERED					":Unauthorized command (already registered) \n"
#define	ERR_NEEDMOREPARAMS(cmd)					"461 " cmd " :Not enough parameters \n"
#define	ERR_PASSWDMISMATCH						"464 :Password incorrect \n"
#define	CH_NICK(o_nick, n_nick)					o_nick + " changed his nickname to " + n_nick + "\n"
#define ERR_BADCHANNELKEY(channel)				"475 " + channel + " :Cannot join channel (+k) \n"
#define RPL_NAMREPLY(client, channels, admin)	"353 * " + client + " + " + channels + admin + "\n"
#define RPL_ENDOFNAMES(client, channel)			"366 * " + client + " " + channel + " :End of /NAMES list. \n"

#endif //IRCSERV_MESSAGES_HPP
