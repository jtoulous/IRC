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
  size_t pos = entry.find('#'); //cherche le channel dans le entry
  guest = entry.substr(0, pos - 1); //recupere l'invité
  channel = findChannelInString(entry); 
  if (guest.empty() || channel.empty()){ //verifie au'il y est un invité et un channel 
    sendMsg(ERR_NEEDMOREPARAMS(client->getNickname(), entry), client->getFd(), client->getNickname());
    return;
  }
  if (IfChannelExist(channel) == false){ //vérifie que le channel existe
      std::cout << "Channel no exist" << std::endl;
        sendMsg(ERR_NOSUCHCHANNEL(client->getNickname()), client->getFd(), client->getNickname());
        return ;
  }
  else if (IfGuestExist(guest) == NULL){//vérifie que l'invité est dans le serveur
    std::cout << "Guest not on server" << std::endl;
    sendMsg(ERR_NOSUCHNICK(guest), client->getFd(), client->getNickname());
  }
  else{
    int index_chan = Utils::findChannelIndex(channel, this->channelList); //cherche le channel dans le vecteur
    if (this->channelList[index_chan]->getOwner() == client->getFd() || this->channelList[index_chan]->FdIsAdmin(client->getFd()) == true){//check si l'invitant et admin ou opérateur
      Client* guestClient = IfGuestExist(guest);//stock l'invité dans un nouveau client
      sendMsg(RPL_INVITING(client->getNickname(), this->channelList[index_chan]->getName(), guestClient->getNickname()), client->getFd(), client->getNickname());//envoie l'invit 
      SendMessageToClient(guestClient->getFd(), guestClient, index_chan);
      this->GuestList.push_back(guestClient); //ajoute l'invité a un vecteur d'invité dans le channel
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