#ifndef DEFINE_HPP 
#define DEFINE_HPP

//Colors
#define GREEN "\033[32m"
#define ORANGE "\033[1;33m"
#define RED "\033[1;31m"
#define DEFAULT "\033[0m"

//Command responses
#define RPL_WELCOME(client) (": 001 " + client + ": Welcome to the Internet Relay Network\r\n")
#define RPL_NOTOPIC(client, channel) (": 331" + client + " " + channel + " :No topic is set\r\n")
#define RPL_TOPIC(client, channel, topic) (": 332 " + client + " " + channel + " :" + topic + "\r\n")
#define RPL_GUESTINVITE(client, channel) (client + " INVITES YOU IN " + channel + "\r\n")
#define RPL_INVITING(client, channel, guest) (": 341 " + client + " INVITE " + guest + " IN " + channel + "\r\n")
#define RPL_PRIVMSG_DEST(srcName, destName, msg) (":" + srcName + " PRIVMSG " + destName + " :" + msg + "\r\n")
#define RPL_PRIVMSG_SRC(srcName, msg) (": 302 " + srcName + " :" + msg + "\r\n") //message sent confirmation
#define RPL_NICKCHANGE(newNick) (": 001 " + newNick + " :Nickname changed successfully\r\n")
#define RPL_NEWOPERATOR(nickname, channel) (": 381 " + nickname + " :You are now operator on " + channel + "\r\n")
#define RPL_NEWOPERATORFOROWNER(owner, nickname, channel) (": 381 " + owner + " :" + nickname + " is now now operator on " + channel + "\r\n")
#define RPL_NEWOPERATORFORCHANNEL(nickname, channel) (":Info PRIVMSG " + channel + " :" + nickname + " is now an operator\r\n")
#define RPL_DEOPPED(nickname, channel) (": 381 " + nickname + " :You are now deopped in " + channel + "\r\n")
#define RPL_DEOPPEDFOROWNER(owner, nickname, channel) (": 381 " + owner + " :" + nickname + " is now deopped in " + channel + "\r\n")
#define RPL_DEOPPEDFORCHANNEL(nickname, channel) (":Info PRIVMSG " + channel + " :" + nickname + " was deopped\r\n")
#define RPL_ALREADYOPERATOR(owner, channel, newOperator) (": 368 " + owner + " " + channel + " " + newOperator + " :User is already operator")
#define RPL_NOTOPERATOR(owner, channel, victim) (": 368 " + owner + " " + channel + " " + victim + " :User is not an operator")
#define RPL_NOTICE(client) (": NOTICE " + client + " you are not iidentified\r\n" )

//Error Replies
#define RPL_USERHOST(client, user) (": 302 " + client + " :" + user + "\r\n")
#define RPL_NAMREPLY(channel, client, allClient) (": 353 " + client + " = " + channel + " : " + allClient + "\r\n")
#define ERR_NOSUCHNICK(client) (": 401 " + client + " :No such nick/channel\r\n")
#define ERR_BADNICKNAME(client) (client + " Bad nickname definition\r\n")
#define ERR_BADUSERNAME(client) (client + " Bad username definition\r\n")
#define ERR_NOSUCHCHANNEL(client) (": 403 " + client + " :No such channel\r\n")
#define ERR_CANNOTSENDTOCHAN(client, channel) (": 404 " + client + " " + channel + " :Cannot send to channel\r\n") 
#define ERR_NORECIPIENT(client) (": 411 " + client + ":No recipient given PRVMSG\r\n")
#define ERR_NOTEXTTOSEND(client) (": 412 " + client + ":No text to send\r\n")
#define ERR_UNKNOWNCOMMAND(client, cmd)(": 421 " + client + " " + cmd + " :Unknown command\r\n")
#define ERR_ERRONEUSNICKNAME(client, nick)(": 432 " + client + " " + nick + " :Erroneus nickname\r\n")
#define ERR_USERNOTINCHANNEL(client, nick, channel)(": 482 " + client + " " + channel + " " + nick + " :They aren't on that channel\r\n")
#define ERR_NOTONCHANNEL(client, channel)(": 442 " + client + " " + channel + " :They aren't on that channel\r\n")
#define ERR_NOTREGISTERED(client)(": 451 " + client + " :You have not registered\r\n")
#define ERR_NEEDMOREPARAMS(client, cmd)(": 461 " + client + " " + cmd + " :Not enough parameters\r\n")
#define ERR_ALREADYREGISTRED(client)(": 462 " + client + ":You may not reregister\r\n")
#define ERR_PASSWDMISMATCH(client)(": 464 " + client + ":Password incorrect\r\n")
#define ERR_CHANNELISFULL(client, channel)(": 471 " + client + " " + channel + " :Cannot join channel (+l)\r\n")
#define ERR_INVITEONLYCHAN(client, channel)(": 473 " + client + " " + channel + "  :Cannot join channel (+i)\r\n")
#define ERR_BADCHANNELKEY(client, channel)(": 475 " + client + " " + channel + " :Cannot join channel (+k)\r\n")
#define ERR_CHANOPRIVSNEEDED(client, channel) (": 482 " + client + " " + channel + "  :You're not channel operator\r\n")
#define ERR_NOTOWNER(client, channel) (": 482 " + client + " " + channel + "  :You're not channel owner\r\n")
#define ERR_NICKALREADYUSED(nickname) (": 433 * " + nickname + " :Nickname already in use")

#endif