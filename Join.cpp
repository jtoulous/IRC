#include "Server.hpp"

int passSpace(String str) {

    int i = 0;
    while (str[i] && str[i] == ' ')
        i++;
    return (i);
}

/* probleme valeur des variable de client, nickname vide */
void    Server::join(Client *client, String cmd, String entry) {
    
    String all = entry.substr(cmd.size(), entry.find('\0'));

    int owner;
    String name = all.substr(passSpace(all), all.find(' ', passSpace(all)));
    String password = all.substr(name.size() + passSpace(all), all.find('\0', name.size()));
    if (CheckChannelName(name) == false) {
        std::cout << "Push back KO" << std::endl;
        String error = ":The_serveur 403 * #channel:No such channel +tn\r\n";
        send (client->getFd(), error.c_str(), error.size(), 0);
        return ;
    }
    if (IfChannelExist(name) == true) {
        /* Check si le channel est sur invation ou pas a remettre quand MODE sera fait */
        /*if (this->channelList[index]->getInviteOnly() == true) {
            std::cout << "Error channel invite only" << std::endl; 
            return ;    
        }*/
        std::cout << "Push_back KO" << std::endl;
        //String allcommands = this->channelList[index]->PrintCommandCanalForUser();
        //send(client->getFd(), allcommands.c_str(), allcommands.size(), 0);
        //send (client->getFd(), ":The_serveur 324 * #channel +tn\r\n", 33, 0);
        return ;
    }
    owner = client->getFd();
    this->channelList.push_back(new Channel(name, password, owner));
    /* Ajouter fonction qui retourne un index et pour recuperer le bon nom de channel */
    std::cout << "Push_back OK" << std::endl;
    std::string message_user = ":" + client->getNickname() + " JOIN " /*+ this->channelList[index]->getName() */+ "\r\n";
    std::cout << "message = " << message_user << std::endl;
    send (client->getFd(), message_user.c_str(), message_user.size(), 0);
  //  String allcommands = this->channelList[index]->PrintCommandCanalForUser();
    //send(client->getFd(), allcommands.c_str(), allcommands.size(), 0);

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