#pragma once

#include "Server.hpp"

#define vector          std::vector
class Server;

class Channel
{
    public:
            Channel();
            Channel(String name, String password, int admin);
            bool        CheckChannelName(String name);
            //bool        IfChannelExist(String name, int *index);
            void        AddMembers(String name, String password);

            String        PrintCommandCanalForOwner();

            String      getName();
            String      getPassword();
            String      getTopic();
            bool        getInviteOnly();
    private:
            String  name;
            String  password;
            String  topic;
            int     owner;
            bool    invite_only;
            vector<int>    admin;
            vector<int>    users;
};