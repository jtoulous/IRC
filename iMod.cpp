#include "Server.hpp"


/*  MODE #Finnish +i  */
void    iMod(Channel *channel, Client *Target, String mode) {

    String msg;
    if (mode == "+i") {
        channel->setBoolInviteOnly(true);
        msg = "MODE " + channel->getName() + " +i" + "\r\n";
        send(Target->getFd(), msg.c_str(), msg.size(), 0);
    }
    else if (mode == "-i") {
        channel->setBoolInviteOnly(false);
        msg = "MODE " + channel->getName() + " -i" + "\r\n";
        send(Target->getFd(), msg.c_str(), msg.size(), 0);
    }
}