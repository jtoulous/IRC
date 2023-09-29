#include "Server.hpp"

/////////////////////////////////////////////////
//                 CONSTRUCTORS                //
/////////////////////////////////////////////////

Server::Server()
{}

Server::Server(char **argv)
: Hostname("Server")
{
    sockaddr_in serverAddr;
    int reuse = 1;

    EntrySocket = socket(AF_INET, SOCK_STREAM, 0);
    if (EntrySocket == -1)
        throw (Xception("Error: entry socket not created"));
    
    setsockopt(EntrySocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[1]));
    if (bind(EntrySocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
        throw (Xception("Error: binding entry socket"));

    password = argv[2];
    
    std::cout << "\n================================\n"
              <<   "|       SERVER ACTIVE          |\n" 
              <<   "================================\n" 
              << std::endl;
}

Server::~Server()
{
    close (EntrySocket);
}


////////////////////////////////////////////////
///             GETTERS                       //
////////////////////////////////////////////////

int     Server::getFdMax()
{
    int Max = EntrySocket;

    for (int i = 0; i < (int)clientList.size(); i++)
        if (clientList[i]->getFd() > Max)
            Max = clientList[i]->getFd();
    return (Max);
}

int     Server::getEntrySocket()
{
    return (EntrySocket);
}

//////////////////////////////////////////////////////
//                  METHODS                         //
//////////////////////////////////////////////////////


void    Server::servEmpty()
{
    int             newClient;
    sockaddr_in     clientAddr;
    socklen_t       clientSize = sizeof(clientAddr);

    listen(EntrySocket, 10);
    newClient = accept(EntrySocket, (struct sockaddr *)&clientAddr, &clientSize);
    if (newClient == -1)
        throw (Xception("Error: accept()"));
    clientList.push_back(new Client);
    clientList[clientList.size() - 1]->setFd(newClient);
    clientList[clientList.size() - 1]->setNb(0);
    //send(newClient, "password: ", 10, 0);
}

void    Server::servCheckSockets(fd_set &sockets)
{
    if (FD_ISSET(EntrySocket, &sockets))
        servNewConnection();
    for (unsigned int i = 0; i < clientList.size(); i++)
        if (FD_ISSET(clientList[i]->getFd(), &sockets))
            servTreatClient(clientList[i]);
}

void    Server::servNewConnection()
{
    int             newClient;
    sockaddr_in     clientAddr;
    socklen_t       clientSize = sizeof(clientAddr);

    newClient = accept(EntrySocket, (struct sockaddr *)&clientAddr, &clientSize);
    if (newClient == -1)
        throw (Xception("Error: accept()"));
    clientList.push_back(new Client);
    clientList[clientList.size() - 1]->setFd(newClient);
    clientList[clientList.size() - 1]->setNb(clientList.size() - 1);
    //send(newClient, "password: ", 10, 0);
}

void    Server::servTreatClient(Client *client)
{
    String  entry;
    String  cmd;

    servReceive(client);
    
    while (client->buffer.find('\n') != NPOS)
    {
        entry = client->buffer.substr(0, client->buffer.find('\n'));
        client->buffer.erase(0, client->buffer.find('\n') + 1);
        
        if (entry.find(' ') == NPOS)
            send(client->getFd(), "bad input\n", 10, 0);
        else
        {
            cmd = Utils::getCmd(entry);

            if (cmd == "PASS")
                pass(client, entry);
            else if (cmd == "NICK")
                nick(client, entry);
            else if (cmd == "USER")
                user(client, entry);
            //else if (cmd == "PRIVMSG")
            //    privMsg(client, entry);
            else if (cmd == "JOIN")
                join(client, entry);
            //else if (CMD == "INVITE")  
        } 
    }


}

void    Server::servReceive(Client *client)
{
    int     size = BUFFER_SIZE;
    char    buff[BUFFER_SIZE];

    while (size == BUFFER_SIZE)
    {
        size = recv(client->getFd(), buff, 100, 0);   
        if (size == -1)
        {
            std::cout << "recv problem for " << client->getUsername() << std::endl;
            return;
        }
        buff[size] = '\0';
        client->buffer = client->buffer + buff;
    }
    std::cout << client->buffer << std::endl;
    client->buffer -= "\r";
}


/////////////////////////////////////////////////////
//              IRC COMMANDS                       //
////////////////////////////////////////////////////


void    Server::pass(Client *client, String &entry)
{
    String  entryPwd;

    entryPwd = entry.substr(entry.find(' ') + 1, entry.size());
    if (entryPwd == password)
    {
        client->setLoggedIn(1);
        std::cout << "new client connected" << std::endl;
        send(client->getFd(), ":The_server 001 * :Welcome on the server\r\n", 42, 0);
    }
}

void    Server::nick(Client *client, String &entry)
{
    String  nickname;
    int     pos;

    pos = entry.find(' ') + 1;
    nickname = entry.substr(pos, entry.find(' ', pos));

    client->setNickname(nickname);
    send(client->getFd(), "Nickname changed successfully\r\n", 31, 0);
    std::cout << "client " << client->getNb() << ": nickname set to " << client->getNickname() << std::endl;
}

void    Server::user(Client *client, String &entry)
{
    String  username;
    int     pos;

    pos = entry.find(' ') + 1;
    username = entry.substr(pos, entry.find(' ', pos) - pos);
    client->setUsername(username);
    send(client->getFd(), "Username changed successfully\r\n", 31, 0);
    std::cout << "client " << client->getNb() << ": username set to " << client->getUsername() << std::endl;
}


void    Server::join(Client *client, String &entry)
{
    (void)entry;
    send (client->getFd(), ":The_serveur 324 * #channel +tn\r\n", 33, 0);
    std::cout << "was send" << std::endl;
}

//void    Server::ping()
//{}

//void    Server::invite()
//{}