#include "Server.hpp"

void    Server::pass(Client *client, String &entry)
{
    String  entryPwd = entry.getWord(2);

    if (entryPwd == password)
    {
        client->setLoggedIn(1);
        sendMsg(RPL_WELCOME(client->getNickname()), client->getFd(), client->getNickname());
    }
    //else //il degage
    //{
    //    close (client->getFd());
    //    Utils::removeClient(client->getFd(), clientList);
    //}
}

void    Server::nick(Client *client, String &entry)
{
    String  nickname = entry.getWord(2);

    if (!Utils::nicknameAvailable(nickname, clientList))
        sendMsg(ERR_NICKALREADYUSED(nickname), client->getFd(), client->getNickname());
    else
    {
        client->setNickname(nickname);
        sendMsg(RPL_NICKCHANGE(nickname), client->getFd(), client->getNickname());
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
        sendMsg("Username changed successfully\r\n", client->getFd(), client->getNickname());
    }
}