#include "Channel.hpp"

/*Commande: KICK
Paramètres: <canal> <utilisateur> [<commentaire>]

La commande KICK est utilisée pour retirer par la force un utilisateur d'un canal (PART forcé).

Seul un opérateur de canal peut kicker un autre utilisateur hors d'un canal. Tout serveur qui reçoit un message KICK vérifie si le message est valide (c'est-à-dire si l'expéditeur est bien un opérateur du canal) avant d'ôter la victime du canal.

Réponses numériques :

           ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
           ERR_BADCHANMASK                 ERR_CHANOPRIVSNEEDED
           ERR_NOTONCHANNEL
Exemples:

KICK #Finnish John :Speaking English ; Kick John de #Finnish en spécifiant "Speaking English" comme raison (commentaire).
:WiZ KICK #Finnish John ; Message KICK de WiZ pour retirer John du canal #Finnish*/


/* KICK #Finnish John :Speaking English */

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



void    Server::kick(Client *client, String &entry) {

    (void)client;
    entry.rmWord(1);
    entry.rmWord(1);
    String name_chan = entry.substr(0, entry.find(' ', Utils::passSpace(entry)));
    
    int i = 0;
    i = ParseName(entry, name_chan, i);
    if (entry[i] && entry[i] == ':')
        i++;
    int j = 0;
    while (entry[i] && entry[i] != ' ') {
        i++;
        j++;
    }
    i -= j;
    String name_user = entry.substr(i, j);
    i += name_user.size();
    while (entry[i] && entry[i] == ' ')
        i++;
    String message_kick = entry.substr(i, entry.find('\0', name_user.size()));
    /*std::cout << RED << "name_channel = " << name_chan << std::endl;
    std::cout << RED << "name_user = " << name_user << std::endl;
    std::cout << RED << "message_kick = " << message_kick << std::endl;*/
    
    int index_chan = 0;
    if (IfChannelExist(name_chan) == true)
        index_chan = Utils::findChannelIndex(name_chan, channelList);
    /* verfier les droit */
    if (this->channelList[index_chan]->getOwner() != client->getFd()) {
        if (this->channelList[index_chan]->FdIsAdmin(client->getFd()) == false) {
            sendMsg(ERR_CHANOPRIVSNEEDED(client->getNickname(), this->channelList[index_chan]->getName()), client->getFd(), client->getNickname());
            return ;
        }
    }
    /* si le message du kick est vide */
    String message_client;
    int client_fd = client->getFd();
    /*  :WiZ KICK #Finnish John ; Message KICK de WiZ pour retirer John du canal #Finnish */
    if (message_kick.empty()) {
        std::cout << "KICK ok" << std::endl;
        message_client = ":" + client->getNickname() + " KICK " + this->channelList[index_chan]->getName() + " " + name_user + " :T pa bo \r\n";
        sendMsg(message_client, client_fd, client->getNickname());
        //send (client_fd, message_client.c_str(), message_client.size(), 0);
    }
    else {
        std::cout << "KICK ok" << std::endl;
        message_client = ":" + client->getNickname() + " KICK " + this->channelList[index_chan]->getName() + " " + name_user + " :T pa bo \r\n";
        sendMsg(message_client, client_fd, client->getNickname());
        //send (client_fd, message_client.c_str(), message_client.size(), 0);
    }
}