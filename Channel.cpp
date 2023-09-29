#include "Channel.hpp"

Channel::Channel() {}

Channel::Channel(String n, String pwd, int adm): name(n), password(pwd), topic(""), owner(adm) {}
/*
String    Channel::PrintCommandCanalForUser() {

    String str = "PRIVMSG " + this->name + " :All commands avalaible: \n"
                "PASS: Is used to set a 'connection password.\n"
                "NICK: Is used to give user a nickname or change the previous one.\n"
                "USER: Is used to specify a user's details when connecting to an IRC server.\n"
                "JOIN:  is used to join or create a specific channel (chat room) on the IRC server.\n"
                "PRIVMSG: is used to send a private message to a specific user or a message to a channel.\n"
                "INVITE:  is used to invite a specific user to join a channel.\n" ;
    return (str);
}*/
String      Channel::getName() {
    return (this->name);
}

String      Channel::getPassword() {
    return (this->password);
}

String      Channel::getTopic() {
    return (this->topic);
}

bool        Channel::getInviteOnly() {
    return (this->invite_only);
}