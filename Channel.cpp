#include "Channel.hpp"

Channel::Channel() {}

Channel::Channel(String n, String pwd, int adm): name(n), password(pwd), topic(""), owner(adm) {
    users.push_back(adm);
    topic_privilege = true;
    limit_bool = false;
}

Channel::~Channel() {}

String      Channel::getName() {
    return (this->name);
}

String      Channel::getPassword() {
    return (this->password);
}

String      Channel::getTopic() {   
    return (this->topic);
}

bool        Channel::getInviteOnly() {
    return (this->invite_only);
}

void    Channel::setUserFd(int user_fd) {

    this->users.push_back(user_fd);
}

void    Channel::setAdminFd(int fd)
{
    if (!FdIsAdmin(fd))
        admin.push_back(fd);
}

vector<int> &Channel::getUsers()
{
    return (users);
}

vector<int> &Channel::getAdmins()
{
    return (admin);
}

int         &Channel::getOwner()
{
    return (owner);
}

int         Channel::getAdminWithIndex(int index_vec) {
    return (this->admin[index_vec]);
}

bool        Channel::FdIsAdmin(int fd) {

    if (this->admin.size() == 0)
        return (false);
    for (size_t i = 0; i < this->admin.size(); i++) {
        if (this->admin[i] == fd)
            return (true);
    }
    return (false);
}

bool    Channel::FdIsUser(int fd)
{
    for (int i = 0; i < (int)users.size(); i++)
        if (fd == users[i])
            return (true);
    return (false);
}

bool    Channel::FdIsOwner(int fd)
{
    if (fd == owner)
        return (true);
    return (false);
}

void    Channel::diffuseMsg(String msg, vector<Client *> clientList, int srcFd)
{
    for (int i = 0; i < (int)users.size(); i++)
        if (srcFd != users[i])
            sendMsg(msg, users[i], Utils::findClientNick(users[i], clientList));
}

void    Channel::setBoolInviteOnly(bool invite) {
    this->invite_only = invite;
}

void        Channel::setTopic(String topik) {
    this->topic = topik;
}

void    Channel::removeAdmin(int fd)
{
    for (int i = 0; i < (int)admin.size(); i++)
    {
        if (admin[i] == fd)
        {
            admin.erase(admin.begin() + i);
            return;
        }
    }
}

void        Channel::setTopicPrivilege(bool privilege) {
    this->topic_privilege = privilege;
}

bool    Channel::getTopicPrivilege() {
    return (this->topic_privilege);
}

void    Channel::setPassword(String pass) {
    this->password = pass;
}

void    Channel::setLimitUsers(int limits) {
    if (limits < 0)
        this->limit_users = 0;
    else
        this->limit_users = limits;
}

int    Channel::getLimitUsers() {
    return (this->limit_users);
}

void    Channel::addCountUsers(int nb) {
    this->count_users += nb;
}

int     Channel::getCountUsers() {
    return (this->count_users);
}

void        Channel::setBoolLimitUsers(bool l) {
    this->limit_bool = l;
}

bool    Channel::getBoolLimitUsers() {
    return (this->limit_bool);
}