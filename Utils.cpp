#include "Utils.hpp"

Utils::Utils()
{}

String  Utils::getCmd(String entry)
{
    String cmd;

    cmd = entry.substr(0, entry.find(' '));
    return (cmd);
}

int     Utils::findClientFd(String &nickname, vector<Client *> &clientList)
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

String  Utils::findClientNick(int fd, vector<Client *> &clientList)
{
    String  notFound;

    for (int i = 0; i < (int)clientList.size(); i++)
    {
        if (clientList[i]->getFd() == fd)
            return (clientList[i]->getNickname());
    }
    return (notFound);
}

int     Utils::findChannelIndex(String &name, vector<Channel *> &channelList)
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

bool    Utils::nicknameAvailable(String &nickname, vector<Client *> &clientList)
{
    for (int i = 0; i < (int)clientList.size(); i++)
        if (clientList[i]->getNickname() == nickname)
            return (false);
    return (true);
}

bool    Utils::usernameAvailable(String &username, vector<Client *> &clientList)
{
    for (int i = 0; i < (int)clientList.size(); i++)
        if (clientList[i]->getUsername() == username)
            return (false);
    return (true);
}

/*void    Utils::rmFromServer(Client *client, vector<Client *> &clientList, vector<Channel *> &channelList)
{
    int fdTarget = client->getFd();

    for (int i = 0; i < channelList.size(); i++) //checker les channel ou il etait
    {
        vector<int> users = channelList[i]->getUsers();
        vector<int> admins = channelList[i]->getAdmins();
        int         owner = channelList[i]->getOwner();

        for (int u = 0; u < users.size(); u++)
        {
            if (users[i] == fdTarget)
            {
                users.erase(users.begin() + i);           
                break;
            }
        }

        for (int a = 0; a < admins.size(); a++)
        {
            if (admins[i] == fdTarget)
            {
                admins.erase(admins.begin() + i);
                break;
            }
        }

        if (owner == fdTarget)
        {
            if (admins.size() != 0)
                owner = admins[0];
            else if (users.size() != 0)
                owner = users[0];
            else
            {
                channelList.erase(channelList.begin() + i);
                i--;
            }
        }
    }

    for (int i = 0; i < clientList.size(); i++)
    {
        if (clientList[i]->getFd() == fdTarget)
        {    
            clientList.erase(clientList.begin() + i);
            break;
        }
    }

    close (fdTarget);
}
*/