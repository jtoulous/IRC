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

            static int     findClientFd(String &nickname, vector<Client *> clientList);
            static int     findChannelIndex(String &name, vector<Channel *> channelList);

    private:
            Utils();
            //Utils(const Utils &ref);
            //~Utils();
            //Utils &operator=(const Utils &ref);
};