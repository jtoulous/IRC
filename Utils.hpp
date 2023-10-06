#pragma once

#include "libX.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"

class Client;
class Server;
class Channel;

#define vector          std::vector

class Utils
{
    public:
            static String  getCmd(String entry);

            static int          findClientFd(String &nickname, vector<Client *> &clientList);
            static String       findClientNick(int fd, vector<Client *> &clientList);
            static int          findClientIndex(int fd, vector<Client *> &clientList);
            static int          findChannelIndex(String &name, vector<Channel *> &channelList);
            static bool         nicknameAvailable(String &nickname, vector<Client *> &clientList);
            static bool         usernameAvailable(String &username, vector<Client *> &clientList);
            //static void         rmFromServer(Client *client, vector<Client *> &clientList, vector<Channel *> &channelList);
            static int          passSpace(String str);
            bool                modValidChar(char ch);
    private:
            Utils();
            //Utils(const Utils &ref);
            //~Utils();
            //Utils &operator=(const Utils &ref);
};