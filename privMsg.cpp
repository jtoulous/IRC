#include "Server.hpp"

static void privmsg_toUser(Clients *client, String &entry, std::vector<Clients *> clientList)
{
    String  msg;
    String  destNickname = entry.getWord(1);
    int     destFd       = Utils::findClientFd(destNickname);

    if (destFd == -1)//le destinataire n'existe pas
    {
        msg.bigJoin(":Server 401 ", client.getNickname().c_str(), " :No such user\r\n", NULL, NULL);
        send(client.getFd(), msg.c_str(), msg.size(), 0);//client does not exist
    }

    if (entry.wordCount() == 2)//msg ne fait que un mot
    {
            msg.bigJoin(":", client->getNickname().c_str(), " PRIVMSG ", destNickname.c_str(), " :");
            msg += entry.getWord(2) + "\r\n";
            send(Utils::findClientFd(entry.getWord(1)), msg.c_str(), msg.size(), 0);
    }

    else
    {
        msg.bigJoin(":", client->getNickname().c_str(), " PRIVMSG ", destNickname.c_str(), " :");
        for (int i = 2; i < entry.wordCount(); i++)
            msg += entry.getWord(i) + " ";
        msg += "\r\n"

        send(destFd, msg.c_str(), msg.size(), 0);
    } 
}

//static void privmsg_toChannel(Client *client, String &entry)
//{}

static int  privmsg_checkFormat(String &entry)
{
    int     type = 0;
    int     nbWords = entry.wordCount();
    int     i;

    for (i = 0; entry[i] == ' '; i++) ;

    if (entry[i] == '#')
        type = 1;
    else
        type = 2;
    
    if (nbWords <= 1)
        return (0);

    if (nbWords == 2)
        return (type);

    if (entry.getWord(2) != ":")
        return (0);

    return (type);
}

void    Server::privMsg(Client *client, String &entry)
{
    entry -= "privmsg ";

    if (privmsg_checkFormat(entry) == 1)
        privmsg_toChannel(client, entry);
    else if (privMsg_checkFormat(client, entry) == 2)
        privmsg_toUser(client, entry, clientList);
    else
        send(client->getFd(), "privmsg: bad input", 18, 0);    
}