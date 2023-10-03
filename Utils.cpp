#include "Utils.hpp"

Utils::Utils()
{}

String  Utils::getCmd(String entry)
{
    String cmd;

    cmd = entry.substr(0, entry.find(' '));
    return (cmd);
}

int     Utils::findClientFd(String &nickname, vector<Client *> clientList)
{
    size_t i = 0;
    
    while(i != clientList.size())
    {
        if (clientList[i]->getNickname() == nickname)
            return (clientList[i]->getFd());
        i++;
    }
    return (-1);
}

String  Utils::findClientNick(int fd, vector<Client *> clientList)
{
    String  notFound;

    for (int i = 0; i < (int)clientList.size(); i++)
    {
        if (clientList[i]->getFd() == fd)
            return (clientList[i]->getNickname());
    }
    return (notFound);
}

int     Utils::findChannelIndex(String &name, vector<Channel *> channelList)
{
    size_t i = 0;
    
    while(i != channelList.size())
    {
        if (channelList[i]->getName() == name)
            return (i);
        i++;
    }
    return (-1);
}