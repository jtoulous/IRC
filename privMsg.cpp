#include "Server.hpp"

static void privmsg_toUser(Clients *client, String &entry, std::vector<Clients *> clientList)
{
    String  destUser;
    String  msg;
    int     i = 0;

    if (entry.wordCount() == 2)
    {
        if (Utils::findClientFd(entry.getWord(1)) == -1)
        {
            msg.bigJoin(":Server 404 ", client.getNickname().c_str(), " : no such user", NULL, NULL);
            send(client.getFd(), msg.c_str(), msg.size(), 0);//client does not exist
        }
        else
        {
            msg.bigJoin(client->getNickname().c_str(), )
            send(findClientFd(entry.getWord(1)));
        }
    }



    destUser = entry.getWord(1);
    for (int i = 2; )

    while (clientList[i])
    {

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