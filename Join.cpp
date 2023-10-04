#include "Server.hpp"

int passSpace(String str) {

    int i = 0;
    while (str[i] && str[i] == ' ')
        i++;
    return (i);
}

/* probleme valeur des variable de client, nickname vide */
void    Server::join(Client *client, String &entry) {
    
    int owner_fd = -1;
    int index_chan = -1;

    String all = entry;
    all.rmWord(1);
    //String all = entry.substr(cmd.size(), entry.find('\0'));
    String name = all.substr(passSpace(all), all.find(' ', passSpace(all)));
    int i = 0;
    
    i = passSpace(all);
    i += name.size();
    while (all[i] && all[i] == ' ')
        i++;
    String password = all.substr(i, all.find('\0', name.size()));
    std::cout << "password = " << "'" << password << "'" << std::endl;
    String message_client;

    if (CheckChannelName(name) == false) {
        std::cout << "Channel no create" << std::endl;
        sendMsg(ERR_NOSUCHCHANNEL(client->getNickname()), client->getFd(), client->getNickname());
        return ;
    }
    if (IfChannelExist(name) == true) {
        /* Check si le channel est sur invation ou pas a remettre quand MODE sera fait */
        /*if (this->channelList[index]->getInviteOnly() == true) {
            std::cout << "Error channel invite only" << std::endl; 
            return ;    
        }*/
        /* si le channel a le bon mot de passe du channel */
        int user_fd = client->getFd();
        index_chan = Utils::findChannelIndex(name, channelList);
        if (IfPasswordIsOk(name, password) == true) {
            SendMessageToClient(user_fd, client, index_chan);
            return ;
        }
        else if (IfPasswordIsOk(name, password) == false) {
            sendMsg(ERR_BADCHANNELKEY(client->getNickname(), this->channelList[index_chan]->getName()), user_fd, client->getNickname());
            return ;
        }

        this->channelList[index_chan]->setUserFd(user_fd);

        SendMessageToClient(user_fd, client, index_chan);
        std::cout << "Push_back KO" << std::endl;
        return ;
    }
    owner_fd = client->getFd();
    
    this->channelList.push_back(new Channel(name, password, owner_fd));
    index_chan = Utils::findChannelIndex(name, channelList);
    std::cout << "Push_back OK" << std::endl;

    SendMessageToClient(owner_fd, client, index_chan);
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
            return (true);
        }
    }

    return (false);
}

bool    Server::IfPasswordIsOk(String name, String password) {

    size_t i = 0;
    while (i < this->channelList.size()) {
        if (this->channelList[i]->getName() == name 
            && !this->channelList[i]->getPassword().empty()) {
                if (this->channelList[i]->getPassword() == password) {
                    std::cout << "password = " << this->channelList[i]->getPassword() << std::endl;
                    return (true);
                }
            }
        i++;
    }
    //std::cout << "password = " << this->channelList[i]->getPassword() << std::endl;
    return (false);
}

void    Server::SendMessageToClient(int client_fd, Client *client, int index_chan) {
    
    String message_client = ":" + client->getNickname() + " JOIN " + this->channelList[index_chan]->getName() + "\r\n";
    send (client_fd, message_client.c_str(), message_client.size(), 0);
}