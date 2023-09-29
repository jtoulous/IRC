#pragma once

#include "libX.hpp"

class Utils
{
    public:
            static String  getCmd(String entry);
            static int     findClientFd(String &nickname, std::vector<Client *> clientList);
    private:
            Utils();
            //Utils(const Utils &ref);
            //~Utils();
            //Utils &operator=(const Utils &ref);
};