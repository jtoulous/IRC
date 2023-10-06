#pragma once

#include "Server.hpp"

#define vector          std::vector
class Server;
class Client;

class Channel
{
    public:
            Channel();
            ~Channel();
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
            int         getAdminWithIndex(int index_vec);

            void        setUserFd(int user_fd);
            void        setBoolInviteOnly(bool invite);
            void        setTopic(String topik);

            bool        FdIsAdmin(int fd);
            bool        FdIsUser(int fd);
            bool        FdIsOwner(int fd);

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