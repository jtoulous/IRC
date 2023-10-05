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
  String guest, channel;
  size_t pos = entry.find('#');
  guest = entry.substr(0, pos - 1);
  channel = findChannelInString(entry);
  if (guest.empty() || channel.empty()){
    sendMsg(ERR_NEEDMOREPARAMS(client->getNickname(), entry), client->getFd(), client->getNickname());
    return;
  }
  if (IfChannelExist(channel) == false){
      std::cout << "Channel no exist" << std::endl;
        sendMsg(ERR_NOSUCHCHANNEL(client->getNickname()), client->getFd(), client->getNickname());
        return ;
  }
  else if (IfGuestExist(guest) == NULL){
    std::cout << "Guest not on server" << std::endl;
    sendMsg(ERR_NOSUCHNICK(guest), client->getFd(), client->getNickname());
  }
  else{
    int index_chan = Utils::findChannelIndex(channel, this->channelList);
    if (this->channelList[index_chan]->getOwner() == client->getFd() || this->channelList[index_chan]->FdIsAdmin(client->getFd()) == true){
      Client* guestClient = IfGuestExist(guest);
      int user_fd = guestClient->getFd();
      sendMsg(RPL_INVITING(client->getNickname(), this->channelList[index_chan]->getName()), client->getFd(), client->getNickname());
     //attend la reponse a faire
      this->channelList[index_chan]->setUserFd(user_fd);
      SendMessageToClient(user_fd, guestClient, index_chan);
      std::cout << "Invite OK\n";
      return;
    }
    else{
      sendMsg(ERR_CHANOPRIVSNEEDED(client->getNickname(), this->channelList[index_chan]->getName()), client->getFd(), client->getNickname());
      std::cout << "Pas les droits\n";
      return;
    }
  }  
}


Client* Server::IfGuestExist(String name){
  if (this->clientList.empty() == true)
    return NULL;
  for(size_t i = 0; i < this->clientList.size(); i++){
    if (name == this->clientList[i]->getNickname())
      return this->clientList[i];
  }
  return NULL;
}