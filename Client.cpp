#include "Client.hpp"

//////////////////////////////////////////
//            BUILDERS                  //
//////////////////////////////////////////


Client::Client()
: fd(-1), loggedIn(false), identified(false), nickname("*")
{}

Client::Client(int in_fd)
: fd(in_fd), loggedIn(false), identified(false), nickname("*")
{}

Client::Client(const Client &ref)
{
    if (this != &ref)
        *this = ref;
}

Client::~Client() {}

///////////////////////////////////////////
//           OPERATORS                   //
///////////////////////////////////////////

Client &Client::operator=(const Client &ref)
{
    this->nb = ref.nb;
    this->fd = ref.fd;
    this->loggedIn = ref.loggedIn;
    this->identified = 0;
    return (*this);
}




///////////////////////////////////////////
//             GETTERS                   //
///////////////////////////////////////////

int     Client::getFd() const
{
    return (fd);
}

int     Client::getLoggedIn() const
{
    return (loggedIn);
}

int     Client::getNb() const
{
    return (nb);
}

int    Client::getIdentification() const
{
    return (identified);
}

String  Client::getNickname() const
{
    return (nickname);
}

String  Client::getUsername() const
{
    return (username);
}

//////////////////////////////////////////
//              SETTERS                 //
//////////////////////////////////////////

void Client::setFd(int val)
{
    fd = val;
}

void    Client::setLoggedIn(int val)
{
    loggedIn = val;
}

void    Client::setNb(int val)
{
    nb = val;
}

void    Client::setIdentification(int val)
{
    identified = val;
}

void    Client::setNickname(String nick)
{
    if (nick.size() == 1 && nick[0] == '*'){
        nickname = nick;
        return;
    }    
    for(size_t i = 0; i != nick.size(); i++){
        if (isalnum(nick[i]) == false){
            nickname = "";
            return;
        }           
    }
    nickname = nick;
}

void    Client::setUsername(String user)
{
    if (user.size() == 1 && user[0] == '*'){
        username = user;
        return;
    }
    for(size_t i = 0; i != user.size(); i++){
        if (isalnum(user[i]) == false){
            username = "";
            return;

        }
    }   
    username = user;
}