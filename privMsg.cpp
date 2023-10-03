#include "Server.hpp"

static void privmsg_toUser(Client *client, String &entry, vector<Client *> clientList)
{
    String  msg;
    String  destNickname = entry.getWord(1);
    int     destFd       = Utils::findClientFd(destNickname, clientList);

    entry.rmWord(1);
    if (destFd == -1)//le destinataire n'existe pas
    {
        sendMsg(ERR_NOSUCHNICK(client->getNickname()), client->getFd());
        return ;
    }

    if (entry.wordCount() == 1)
        msg = entry.getWord(1);
    else
        for (int i = 1; i < entry.wordCount(); i++)
            msg += entry.getWord(i) + " ";
    
    sendMsg(RPL_PRIVMSG_DEST(client->getNickname(), destNickname, msg), destFd);//envoi du msg
    sendMsg(RPL_PRIVMSG_SRC(client->getNickname(), msg), client->getFd());//confirmaion envoi
}

static void privmsg_toChannel(Client *client, String &entry, vector<Channel *> channelList)
{
    String  msg;
    String  destChannel = entry.getWord(1);
    int     channelIdx = Utils::findChannelIndex(destChannel, channelList);

    if (channelIdx == -1)//channel existe pas
    {
        sendMsg(ERR_NOSUCHNICK(client->getNickname()), client->getFd());
        return ;
    }

    if (entry.wordCount() == 2)
        msg = entry.getWord(2);
    else
        for (int i = 2; i < entry.wordCount(); i++)
            msg += entry.getWord(i) + " ";
    
    channelList[channelIdx]->diffuseMsg(RPL_PRIVMSG_DEST(client->getNickname(), destChannel, msg));
    sendMsg(RPL_PRIVMSG_SRC(client->getNickname(), destChannel), client->getFd());//confirmaion envoi
}

static String  privmsg_checkFormat(String &entry)
{
    String      type;
    int         nbWords = entry.wordCount();
    int         i;

    for (i = 0; entry[i] == ' '; i++) ;

    if (entry[i] == '#')
        type = "channel";
    else
        type = "user";
    
    if (nbWords <= 1)
        return ("error");

    if (nbWords == 2)
        return (type);

    if (entry.getWord(2) != ":")
    {
        String word = entry.getWord(1);
        if (word.back() != ':')
        {
            word = entry.getWord(2);
            if (word[0] != ':')
                return ("error")    
        }
    }

    entry.erase(entry.find(':'));
    return (type);
}

void    Server::privMsg(Client *client, String &entry)
{
    entry.rmWord(1);

    if (privmsg_checkFormat(entry) == 1)
        privmsg_toChannel(client, entry, channelList);
    else if (privmsg_checkFormat(entry) == 2)
        privmsg_toUser(client, entry, clientList);
    else
        sendMsg("privmsg: bad input", client->getFd());    
}