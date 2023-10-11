#include "Server.hpp"

int    Server::pass(Client *client, String &entry)
{
    String  entryPwd = entry.getWord(2);

    if (entryPwd == password)
    {
        client->setLoggedIn(true);
        sendMsg(RPL_WELCOME(client->getNickname()), client->getFd(), client->getNickname());
        return (1);
    }
    else //il degage
    {
        close (client->getFd());
        Utils::rmFromServer(client, clientList, channelList, GuestList);
        return (-1);
    }
}

void    Server::nick(Client *client, String &entry)
{
    String  nickname = entry.getWord(2);

    if (!Utils::nicknameAvailable(nickname, clientList))
        sendMsg(ERR_NICKALREADYUSED(nickname), client->getFd(), client->getNickname());
    else
    {
        client->setNickname(nickname);
        if (client->getNickname() == ""){
            std::cerr << "Bad definition NickName." << std::endl;
            return;
        }
        sendMsg(RPL_NICKCHANGE(nickname), client->getFd(), client->getNickname());
        client->setIdentification(true);
    }
}

void    Server::user(Client *client, String &entry)
{
    String  username = entry.getWord(2);

    if (!Utils::usernameAvailable(username, clientList))
        sendMsg(ERR_NICKALREADYUSED(username), client->getFd(), client->getNickname());
    else
    {
        client->setUsername(username);
        if (client->getUsername() == ""){
            std::cerr << "Bad definition UserName." << std::endl;
            return;
        }
        sendMsg("Username changed successfully\r\n", client->getFd(), client->getNickname());
        //sendMsg(RPL_WELCOME(client->getNickname()), client->getFd(), client->getNickname());
    }
}