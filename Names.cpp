#include "Server.hpp"

void    Server::names(Client *client, String &entry) {

    String name_channel = entry.getWord(2);

    if (IfChannelExist(name_channel) == false) {
        sendMsg(ERR_NOSUCHCHANNEL(client->getNickname()), client->getFd(), client->getNickname());
        return ;
    }
    int i = 0;
    String allName;
    while (i < (int)this->clientList.size()) {
        allName += this->clientList[i]->getNickname();
        allName += " ";
        i++;
    }
    sendMsg(RPL_NAMREPLY(name_channel, client->getNickname(), allName), client->getFd(), client->getNickname());
}