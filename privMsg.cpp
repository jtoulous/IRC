#include "Server.hpp"

static void privmsg_toUser(Client *client, String &entry, vector<Client *> clientList)
{
    String  msg;
    String  destNickname = entry.getWord(1);
    int     destFd       = Utils::findClientFd(destNickname, clientList);

    entry.rmWord(1);
    if (destFd == -1)//le destinataire n'existe pas
    {
        sendMsg(ERR_NOSUCHNICK(client->getNickname()), client->getFd(), client->getNickname());
        return ;
    }

    if (entry.wordCount() == 1)
        msg = entry.getWord(1);
    else
        for (int i = 1; i <= entry.wordCount(); i++)
            msg += entry.getWord(i) + " ";
    
    sendMsg(RPL_PRIVMSG_DEST(client->getNickname(), destNickname, msg), destFd, destNickname);//envoi du msg
    sendMsg(RPL_PRIVMSG_SRC(client->getNickname(), msg), client->getFd(), client->getNickname());//confirmaion envoi
}

static void privmsg_toChannel(Client *client, String &entry, vector<Channel *> channelList, vector<Client *> clientList)
{
    String  msg;
    String  destChannel = entry.getWord(1);
    int     channelIdx = Utils::findChannelIndex(destChannel, channelList);

    entry.rmWord(1);
    if (channelIdx == -1)//channel existe pas
    {
        sendMsg(ERR_NOSUCHNICK(client->getNickname()), client->getFd(), client->getNickname());
        return ;
    }

    if (entry.wordCount() == 1)
        msg = entry.getWord(1);
    else
        for (int i = 1; i <= entry.wordCount(); i++)
            msg += entry.getWord(i) + " ";
    
    channelList[channelIdx]->diffuseMsg(RPL_PRIVMSG_DEST(client->getNickname(), destChannel, msg), clientList, client->getFd());
    sendMsg(RPL_PRIVMSG_SRC(client->getNickname(), destChannel), client->getFd(), client->getNickname());//confirmaion envoi
}

static String  privmsg_checkFormat(String &entry)
{
    String      type;
    String      error = "error";
    int         nbWords = entry.wordCount();

    if (entry.wordStartChar(1) == '#')
        type = "channel";
    else
        type = "user";
    
    if (nbWords <= 1 || entry.wordStartChar(1) == ':')
        return (error);

    if (nbWords == 2)//si y a que 1 mot, gestion d'erreur finit ici
    {    
        if (entry.wordStartChar(2) == ':')
            entry.rmFromWord(2, ':');
        return (type);
    }

    if (entry.getWord(2) != ":")
    {
        if (entry.wordEndChar(1) != ':')
        {
            if (entry.wordStartChar(2) != ':')
                return (error);
            else
                entry.rmFromWord(2, ':');
        }
        else
            entry.rrmFromWord(1, ':');
    }
    else
        entry.rmWord(2);
    return (type);
}

void    Server::privMsg(Client *client, String &entry)
{
    String type;
    entry.rmWord(1);

    type = privmsg_checkFormat(entry);
    if (type == "channel")
        privmsg_toChannel(client, entry, channelList, clientList);
    else if (type == "user")
        privmsg_toUser(client, entry, clientList);
    else
        sendMsg("privmsg: bad input", client->getFd(), client->getNickname());    
}