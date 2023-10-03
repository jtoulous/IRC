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

            void        diffuseMsg(String msg);

            String      getName();
            String      getPassword();
            String      getTopic();
            bool        getInviteOnly();

            void        SetVectorUsers(int users_fd);
    private:
            String  name;
            String  password;
            String  topic;
            int     owner;
            /* variable pour savoir si c'est sur invitation seulement ou pas */
            bool    invite_only;
            vector<int>    admin;
            vector<int>    users;
};