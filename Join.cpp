#include "Server.hpp"

int passSpace(String str) {

    int i = 0;
    while (str[i] && str[i] == ' ')
        i++;
    return (i);
}

/* probleme valeur des variable de client, nickname vide */
void    Server::join(Client *client, String cmd, String entry) {
    
    int owner_fd = -1;
    int index_chan = -1;

    String all = entry.substr(cmd.size(), entry.find('\0'));
    String name = all.substr(passSpace(all), all.find(' ', passSpace(all)));
    String password = all.substr(name.size() + passSpace(all), all.find('\0', name.size()));
    
    String message_client;
    if (CheckChannelName(name) == false) {
        std::cout << "Channel no create" << std::endl;
        sendMsg(ERR_NOSUCHCHANNEL(client->getNickname()), client->getFd());
        return ;
    }
    if (IfChannelExist(name) == true) {
        /* Check si le channel est sur invation ou pas a remettre quand MODE sera fait */
        /*if (this->channelList[index]->getInviteOnly() == true) {
            std::cout << "Error channel invite only" << std::endl; 
            return ;    
        }*/
        int user_fd = Utils::findClientFd(name, this->clientList);
        
        index_chan = Utils::findChannelIndex(name, channelList);
        this->channelList[index_chan]->setUserFd(user_fd);
        message_client = ":" + client->getNickname() + " JOIN " + this->channelList[index_chan]->getName() + "\r\n";
        std::cout << "Push_back KO" << std::endl;
        send (client->getFd(), message_client.c_str(), message_client.size(), 0);
        return ;
    }
    owner_fd = Utils::findClientFd(name, this->clientList);
    
    this->channelList.push_back(new Channel(name, password, owner_fd));
        
    std::cout << "Push_back OK" << std::endl;
    
    message_client = ":" + client->getNickname() + " JOIN " + this->channelList[index_chan]->getName() + "\r\n";
    send (client->getFd(), message_client.c_str(), message_client.size(), 0);
}

/* "PRIVMSG #toncanal :tonmessage" */

bool    Server::CheckChannelName(String name) {
    
    /* voir pour plus de check si il faut */
    if (name[0] == '#' && name[1] != ' ')
        return (true);
    return (false);
}

bool    Server::IfChannelExist(String name) {

    if (this->channelList.empty() == true)
        return (false);
    for (size_t i = 0; i < this->channelList.size(); i++) {
        if (this->channelList[i]->getName() == name) {
            std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAA" << std::endl;
            return (true);
        }
    }

    return (false);
}