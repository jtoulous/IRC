#include "Server.hpp"
#include "Define.hpp"

/////////////////////////////////////////////////
//                 CONSTRUCTORS                //
/////////////////////////////////////////////////

Server::Server()
{}

Server::Server(char **argv, int argc)
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

    if (argc == 3)
        password = argv[2];
    else
        password = "";
    
    std::cout << "\n================================\n"
              <<   "|       SERVER ACTIVE          |\n" 
              <<   "================================\n" 
              << std::endl;
}

Server::~Server()
{}


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

void    Server::servCheckSockets(fd_set &sockets)
{
    if (FD_ISSET(EntrySocket, &sockets))
        servNewConnection();
    /* PROBLEME DE BOUCLE INFINI QUAND le client quitte, le nombre de client ne diminue pas */
    for (unsigned int i = 0; i < clientList.size(); i++) {
        if (FD_ISSET(clientList[i]->getFd(), &sockets))
            servTreatClient(clientList[i]);
    }
}

void    Server::servNewConnection()
{
    int             newClient;
    sockaddr_in     clientAddr;
    socklen_t       clientSize = sizeof(clientAddr);

    if (clientList.size() == 0)
        listen(EntrySocket, 10);
    newClient = accept(EntrySocket, (struct sockaddr *)&clientAddr, &clientSize);
    if (newClient == -1)
        std::cout << RED << "Error: accept()" << DEFAULT << std::endl;
    else
    {
        clientList.push_back(new Client);
        clientList[clientList.size() - 1]->setFd(newClient);
        clientList[clientList.size() - 1]->setNb(clientList.size() - 1);
    }
}

void    Server::servTreatClient(Client *client)
{
    String  entry;
    String  cmd;

    if (servReceive(client) == 0)
    {
        Utils::rmFromServer(client, clientList, channelList, GuestList);
        return ;
    }

    while (client->buffer.find('\n') != NPOS)
    {
        entry = client->buffer.substr(0, client->buffer.find('\n'));
        client->buffer.erase(0, client->buffer.find('\n') + 1);
        
        cmd = entry.getWord(1);
        if (cmd == "PASS" || cmd == "pass")
            pass(client, entry);
        else if (cmd == "NICK" || cmd == "nick")
            nick(client, entry);
        else if (cmd == "USER" || cmd == "user")
            user(client, entry);
        else if (cmd == "JOIN" || cmd == "join")
            join(client, entry);
        else if (cmd == "PRIVMSG" || cmd == "privmsg")
            privMsg(client, entry);
        else if (cmd == "TOPIC" || cmd == "topic")
            Topic(client, entry);
        else if (cmd == "INVITE" || cmd == "invite")
            invite(client, entry);
        else if (cmd == "MODE" || cmd == "mode")
            mode(client, entry);
        else if (cmd == "KICK" || cmd == "kick")
            kick(client, entry);
        else if (cmd == "NAMES" || cmd == "names")
            names(client, entry);
        else if (cmd == "QUIT" || cmd == "quit")
        {
            Utils::rmFromServer(client, clientList, channelList, GuestList);
            return ;    
        }
    }
}

int    Server::servReceive(Client *client)
{
    int     size = BUFFER_SIZE;
    char    buff[BUFFER_SIZE];

    while (size == BUFFER_SIZE)
    {
        size = recv(client->getFd(), buff, 100, 0);   
        if (size == -1)
        {
            std::cout << "recv problem for " << client->getUsername() << std::endl;
            return (-1);
        }
        buff[size] = '\0';
        client->buffer = client->buffer + buff;
    }
    client->buffer -= "\r";
    std::cout << GREEN << "\n" << client->buffer << DEFAULT << std::endl;
    return (size);
}

//////////////////////////////////////////////////////////////////////////

void    sendMsg(String msg, int fd, String nick)
{
    send(fd, msg.c_str(), msg.size(), 0);
    msg -= "\r\n";
    if (!nick.empty())
        std::cout << "=====> " << ORANGE << "to " << nick << ": " << DEFAULT << msg << std::endl;
}