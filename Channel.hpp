#pragma once

#include "Server.hpp"

#define vector          std::vector
class Server;
class Client;

class Channel
{
    public:
            Channel();
            Channel(String name, String password, int admin);
            bool        CheckChannelName(String name);
            bool        IfChannelExist(String name, int *index);
            void        AddMembers(String name, String password);

            void        printMembers();

            String      getName();
            String      getPassword();
            String      getTopic();
            bool        getInviteOnly();
            vector<int> &getUsers();
            vector<int> &getAdmins();
            int         &getOwner();

            void        setUserFd(int user_fd);
            void        setBoolInviteOnly(bool invite);
            
            void        diffuseMsg(String msg, vector<Client *> clientList, int srcFd);
            
    private:
            String  name;
            String  password;
            String  topic;
            int     owner;
            bool    invite_only;
            vector<int>    admin;
            vector<int>    users;
};