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
    
    String name_chan = entry.substr(Utils::passSpace(entry), entry.find(' ', Utils::passSpace(entry)));
    int i = 0;
    i = ParseName(entry, name_chan, i);
    
    String name_user = entry.substr(i, entry.find('\0', name_chan.size()));
    std::cout << RED << "name_channel = " << name_chan << std::endl;
    std::cout << RED << "name_user = " << name_user << std::endl;
}