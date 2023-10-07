#include "Server.hpp"
#include <cstdlib>

int main(int argc, char **argv)
{
    if (argc != 3) {
        std::cerr << "Error format : " << GREEN << "./ircserv " << ORANGE << "<port>" << RED << " <password>" << DEFAULT << std::endl;
        return (1);
    }
    try
    {
        Server          server(argv);   //demarrage serveur
        fd_set          sockets;    //structure qui contient tout les fd des clients et le fd d'entree au serv pour l'utilisation de select()

        while (1)
        {
            if (server.clientList.size() == 0)  //      si y a personne de connecter au serveur ca reste bloquer dans servEmpty(), jusqua que quelqun se connecte      
                server.servNewConnection();             //    
            
            FD_ZERO(&sockets);      //vide le fd_set sockets
            FD_SET(server.getEntrySocket(), &sockets);     //ajoute le fd d'entree au serveur dans le fd_set 
            for (int i = 0; i < (int)server.clientList.size(); i++)   //ajoute le fd de tous les client dans le fd_set
                FD_SET(server.clientList[i]->getFd(), &sockets);
            
            if (select(server.getFdMax() + 1, &sockets, NULL, NULL, NULL) == -1)//surveille tous les fd qui sont dans le fd_set, reste bloquer ici tant que y a aucune activite de detecter
            {    
                perror("perror: ");
                throw (Xception("\nError: select() problemo"));
            }
            server.servCheckSockets(sockets); //check sur quel fd y a de l'activite et traite la demande du client ou la nouvelle connexion entrante
        }
    }

    catch (Xception &e)
    {
        std::cout << e.what() << std::endl;
    }

    return (69);
}