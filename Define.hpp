#ifndef DEFINE_HPP 
#define DEFINE_HPP

//Colors
#define GREEN "\033[32m"
#define ORANGE "\033[1;33m"
#define RED "\033[1;31m"
#define DEFAULT "\033[0m"

//Command responses
#define RPL_WELCOME(client) (": 001 " + client + ": Welcome to the Internet Relay Network\r\n")
#define RPL_NOTOPIC(client, channel)(": 331" + client + " " + channel + " :No topic is set\r\n")
#define RPL_TOPIC(client, channel, topic)(": 332" + client + " " channel + " :" + topic + "\r\n")
#define RPL_INVITING(client, channel , nick)(": 341" + client + " " + channel + " " + nick + "\r\n")
#define RPL_PRIVMSG_DEST(srcName, destName, msg) (":" + srcName + " PRIVMSG " + destName + " :" + msg + "\r\n")
#define RPL_PRIVMSG_SRC(srcName, msg) (": 302 " + srcName + " :" + msg + "\r\n") //message sent confirmation
#define RPL_NICKCHANGE(newNick) (": 001 " + newNick + " :Nickname changed successfully\r\n")

//Error Replies

#define ERR_NOSUCHNICK(client) (": 401 " + client + " :No such nick/channel\r\n")
#define ERR_NOSUCHCHANNEL(client)(": 403" + client + " :No such channel\r\n")
#define ERR_CANNOTSENDTOCHAN(client, channel)(": 404" + client + " " + channel + " :Cannot send to channel\r\n") 
#define ERR_NORECIPIENT(client)(": 411" + client + ":No recipient given PRVMSG\r\n")
#define ERR_NOTEXTTOSEND(client)(": 412" + client + ":No text to send\r\n")
#define ERR_UNKNOWNCOMMAND(client, cmd)(": 421" + client + " " + cmd + " :Unknown command\r\n")
#define ERR_ERRONEUSNICKNAME(client, nick)(": 432" + client + " " + nick + " :Erroneus nickname\r\n")
#define ERR_USERNOTINCHANNEL(client, nick)(": 441" + client + " " + nick + " :They aren't on that channel\r\n")
#define ERR_NOTONCHANNEL(client, channel)(": 442" + client + " " + channel + " :They aren't on that channel\r\n")
#define ERR_NOTREGISTERED(client)(": 451 " + client + " :You have not registered\r\n")
#define ERR_NEEDMOREPARAMS(client, cmd)(": 461" + client + " " + cmd + " :Not enough parameters\r\n")
#define ERR_ALREADYREGISTRED(client)(": 462" + client + ":You may not reregister\r\n")
#define ERR_PASSWDMISMATCH(client)(": 464" + client + ":Password incorrect\r\n")
#define ERR_CHANNELISFULL(client, channel)(": 471" + client + " " + channel + " :Cannot join channel (+l)\r\n")
#define ERR_INVITEONLYCHAN(client, channel)(": 473" + client + " " + channel + "  :Cannot join channel (+i)\r\n")
#define ERR_BADCHANNELKEY(client, channel)(": 475" + client + " " + channel + " :Cannot join channel (+k)\r\n")
#define ERR_CHANOPRIVSNEEDED(client, channel)(": 482" + client + " " channel + "  :You're not channel operator\r\n")
#define ERR_NICKALREADYUSED(nickname) (": 433 * " + nickname + " :Nickname already in use")

#endif