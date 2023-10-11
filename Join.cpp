#include "Server.hpp"


bool    Server::GuestExistForJoin(vector<Client *> GuestClient, Client *client) {

    (void)GuestClient;
    for (int i = 0; i < (int)GuestList.size(); i++) {
        if (GuestList[i]->getNickname() == client->getNickname()) {
            return (true);
        }
    }
    return (false);
}

void    Server::EraseGuestAfterJoin(vector<Client *> GuestClient, Client *client) {

    (void)GuestClient;
    for (int i = 0; i < (int)GuestList.size(); i++) {
        if (GuestList[i]->getNickname() == client->getNickname()) {
            GuestList.erase(GuestList.begin() + i);
            return ;
        }
    }
    return ;
}

/* probleme valeur des variable de client, nickname vide */
void    Server::join(Client *client, String &entry) {
    
    int owner_fd = -1;
    int index_chan = -1;
    String message_client;

    entry.rmWord(1);
    String name = entry.substr(Utils::passSpace(entry), entry.find(' ', Utils::passSpace(entry)));
    
    int i = Utils::passSpace(entry) + name.size();
    while (entry[i] && entry[i] == ' ')
        i++;

    String password = entry.substr(i, entry.find('\0', name.size()));

    if (CheckChannelName(name) == false) {
        sendMsg(ERR_NOSUCHCHANNEL(client->getNickname()), client->getFd(), client->getNickname());
        return ;
    }
    if (IfChannelExist(name) == true) {
        index_chan = Utils::findChannelIndex(name, channelList);        
        if (this->channelList[index_chan]->getInviteOnly() == true) {
            if (GuestExistForJoin(GuestList, client) == false) {
                sendMsg(ERR_INVITEONLYCHAN(client->getNickname(), this->channelList[index_chan]->getName()), client->getFd(), client->getNickname());
                return ; 
            } 
        }
        int user_fd = client->getFd();
        /*  Check si il y a une limite d'utilisateur    */
        if (this->channelList[index_chan]->getBoolLimitUsers() == true) {
            /*  Check la limite du nombre des utilisateur en fonction du MODE +l */
            if (this->channelList[index_chan]->getLimitUsers() == (int)this->channelList[index_chan]->getSizeUsersList()) {
                sendMsg(ERR_CHANNELISFULL(client->getNickname(), this->channelList[index_chan]->getName()), client->getFd(), client->getNickname());
                return ;
            }
        }
        if (PasswordExist(name) == true) {
            if (IfPasswordIsOk(name, password) == true) {
                this->channelList[index_chan]->setUserFd(user_fd);
                SendMessageToClient(user_fd, client, index_chan);
                return ;
            }
            else if (IfPasswordIsOk(name, password) == false) {
                sendMsg(ERR_BADCHANNELKEY(client->getNickname(), this->channelList[index_chan]->getName()), user_fd, client->getNickname());
                return ;
            }
        }
        this->channelList[index_chan]->setUserFd(user_fd);

        SendMessageToClient(user_fd, client, index_chan);
        if (GuestExistForJoin(GuestList, client) == true) {
            EraseGuestAfterJoin(GuestList, client);
        }
        return ;
    }
    owner_fd = client->getFd();
    
    this->channelList.push_back(new Channel(name, password, owner_fd));
    index_chan = Utils::findChannelIndex(name, channelList);

    SendMessageToClient(owner_fd, client, index_chan);
}

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
                    return (true);
                }
            }
        i++;
    }
    return (false);
}

bool    Server::PasswordExist(String name) {

    size_t i = 0;
    while (i < this->channelList.size()) {
        if (this->channelList[i]->getName() == name) {
                if (!this->channelList[i]->getPassword().empty()) {
                    return (true);
                }
            }
        i++;
    }
    return (false);
}

void    Server::SendMessageToClient(int client_fd, Client *client, int index_chan) {
    
    String message_client = ":" + client->getNickname() + " JOIN " + this->channelList[index_chan]->getName() + "\r\n";
    send (client_fd, message_client.c_str(), message_client.size(), 0);
}