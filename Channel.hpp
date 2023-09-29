#pragma once

#include "Server.hpp"

class Channel
{
    public:
            Channel();

    private:
            String  name;
            String  topic;
            String  password;
            int     admin;
            std::vector<int>    members;
            std::vector<int>    loggingIn;
};