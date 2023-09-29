#include "Server.hpp"

static void privMsg_toUser()
{}

static void privMsg_toChannel()
{}

static int  privMsg_checkFormat(String &entry)
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

    if (privMsg_checkFormat(entry) == 1)
        privMsg_toChannel();
    else if (privMsg_checkFormat(entry) == 2)
        privmsg_toUser();
    else
        send(client->getFd(), "privmsg: bad input", 18, 0);    
}