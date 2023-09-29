#include "Utils.hpp"

Utils::Utils()
{}

String  Utils::getCmd(String entry)
{
    String cmd;

    cmd = entry.substr(0, entry.find(' '));
    return (cmd);
}

int     Utils::findClientFd(String &nickname, std::vector<Client *> clientList)
{
    int i = 0;
    
    while(i != clientList.size())
    {
        if (clientList[i].getNickname() == nickname)
            return (clientList[i].getFd());
        i++;
    }
    return (-1);
}

int     Utils::findServerIndex(String &name, std::vector<Channel *> channelList)
{
    int i = 0;
    
    while(i != clientList.size())
    {
        if (channelList[i].getName() == name)
            return (i);
        i++;
    }
    return (-1);
}