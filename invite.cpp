#include "Server.hpp"

static String findChannelInString(String &entry){
size_t pos = entry.find('#');
String channel;
int i = 0;
 if (pos != entry.find('#')){
    std::cout << "Error";
  }
  while (entry[i]){
    if (entry[i] == '#'){
      while (entry[i]){
        channel += entry[i];
        i++;      
      } 
      break; 
    }
    i++;
  }
  return (channel);
}

void  Server::invite(Client *client, String &entry)
{
  entry.rmWord(1);
  (void) client;
  String guest, channel;
  size_t pos = entry.find('#');
  guest = entry.substr(0, pos - 1);
  channel = findChannelInString(entry);
  if (guest.empty() || channel.empty()){
    sendMsg(ERR_NEEDMOREPARAMS(client->getNickname(), entry), client->getFd(), client->getNickname());
    return;
  }
  std::cout <<  "guest:" << guest << "\nchannel:" << channel << std::endl;
  if (IfChannelExist(channel) == false){
      std::cout << "Channel no exist" << std::endl;
        sendMsg(ERR_NOSUCHCHANNEL(client->getNickname()), client->getFd(), client->getNickname());
        return ;
  }
  if (IfGuestExist(guest) == false){
    std::cout << "Guest not on server" << std::endl;
    sendMsg(ERR_NOSUCHNICK(client->getNickname()), client->getFd(), client->getNickname());
  }
  


}


bool Server::IfGuestExist(String name){
  if (this->clientList.empty() == true)
    return false;
  for(size_t i = 0; i < this->clientList.size(); i++){
    if (name == this->clientList[i]->getNickname())
      return true;
  }
  return false;
}