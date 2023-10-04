#include "Server.hpp"

/*  :Wiz TOPIC #test :New topic ;   */

void    Server::Topic(Client *client, String entry) {

    int i = 0;

    String all = entry;
    String name = all.substr(Utils::passSpace(all), all.find(' ', Utils::passSpace(all)));

    i = Utils::passSpace(all);
    i += name.size();
    while (all[i] && all[i] == ' ')
        i++;
    String topic = all.substr(i, all.find('\0', name.size()));
    if (IfChannelExist(name) == true) {
        int index_chan = Utils::findChannelIndex(name, channelList);
        std::cout << "index chan = " << index_chan << std::endl;
        String message_topic = ":" + client->getNickname() + " TOPIC " + this->channelList[index_chan]->getName() + " :" + topic +"\r\n";
        send (client->getFd(), message_topic.c_str(), message_topic.size(), 0);
    }
}