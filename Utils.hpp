#pragma once

#include "libX.hpp"

class Utils
{
    public:
            static String  getCmd(String entry);
            //static int     findClientFd(String &nickname, std::vector<Client *> clientList);
            //static int     findServerIndex(String &name, std::vector<Channel *> channelList)

    private:
            Utils();
            //Utils(const Utils &ref);
            //~Utils();
            //Utils &operator=(const Utils &ref);
};