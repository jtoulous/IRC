#include "Server.hpp"

/*  :Wiz TOPIC #test :New topic ;   */
/*  TOPIC #test :another topic ;    */

static int ParseName(String entry, String name, int i) {

    i = Utils::passSpace(entry);
    i += name.size();
    while (entry[i] && entry[i] == ' ')
        i++;
    if (entry[i] && entry[i] == ':') {
        i++;
    }
    while (entry[i] && entry[i] == ' ')
        i++;
    return (i);
}

/*bool    Server::FdIsAdmin(int fd, int index_chan) {
    if (this->channelList[index_chan]->Fd == 0)
        return (false);
    for (int i = 0; this->channelList[index_chan]->getAdminWithIndex(i); i++) {
        if (this->channelList[index_chan]->getAdminWithIndex(i) == fd)
            return (true);
    }
    return (false);
}*/

void    Server::Topic(Client *client, String entry) {

    int i = 0;


    entry.rmWord(1);
    String name = entry.substr(Utils::passSpace(entry), entry.find(' ', Utils::passSpace(entry)));

    i = ParseName(entry, name, i);

    String topic = entry.substr(i, entry.find('\0', name.size()));
    
    /*  Si le channel existe    */
    if (IfChannelExist(name) == true) {
        int index_chan = Utils::findChannelIndex(name, channelList);
 
        /*  Si le topic rentré en param est vide (String topic au dessus) */
        if (topic.empty()) {
            /*  Si le topic enregistré (la variable topic dans la classe) est vide aussi envoyer RPL_NOTOPIC */
            if (this->channelList[index_chan]->getTopic().empty()) {
                sendMsg(RPL_NOTOPIC(client->getNickname(), this->channelList[index_chan]->getName()), client->getFd(), client->getNickname());
                return ; 
            }
        }
        if (!topic.empty())
        {
            /*  Si le topic n'est pas vide on verifie si il a les droit avec des check du fd owner et admin
                Ex: /TOPIC #chan disussion */
            if (this->channelList[index_chan]->getOwner() != client->getFd()) {
                if (this->channelList[index_chan]->FdIsAdmin(client->getFd()) == false) {
                    sendMsg(ERR_CHANOPRIVSNEEDED(client->getNickname(), this->channelList[index_chan]->getName()), client->getFd(), client->getNickname());
                    return ;
                }
            }
            /*  On envoie la String topic crée au dessus    */
            this->channelList[index_chan]->setTopic(topic);
            sendMsg(RPL_TOPIC(client->getNickname(), this->channelList[index_chan]->getName(), topic), client->getFd(), client->getNickname());
        }
        else {
            /*  Et sinon donc si le topic est vide et que la variable topic de la classe est remplie,
                on l'envoie, pour que le client renvoie le nom du topic actuel.
                Ex: /TOPIC #chan1 */
            String topic_chan = this->channelList[index_chan]->getTopic();
            sendMsg(RPL_TOPIC(client->getNickname(), this->channelList[index_chan]->getName(), topic_chan), client->getFd(), client->getNickname());
        }
    }
}