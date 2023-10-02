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

KICK &Melbourne Matthew ; Kick Matthew de &Melbourne
KICK #Finnish John :Speaking English ; Kick John de #Finnish en spécifiant "Speaking English" comme raison (commentaire).
:WiZ KICK #Finnish John ; Message KICK de WiZ pour retirer John du canal #Finnish*/


/* KICK #Finnish John :Speaking English */

void    Server::kick(Client *client, String &cmd, String &entry) {

    String all = entry.substr(cmd.size(), entry.find('\0'));

    String name_chan = all.substr(passSpace(all), all.find(' ', passSpace(all)));
    String password = all.substr(name.size() + passSpace(all), all.find('\0', name.size()));

    /* tester si ça fonctionne */
    std::cout << "name_channel = " << name_chan << std::endl;
    std::cout << "password = " << password << std::endl;
}