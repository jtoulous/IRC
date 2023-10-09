#include "Server.hpp"                                                      

static void oMod(Channel *channel, Client *owner, Client *target, char operation, vector<Client *> clientList)
{
  if (operation == '+')
  {
      if (!channel->FdIsAdmin(target->getFd()))
      {
        channel->setAdminFd(target->getFd());
        sendMsg(RPL_NEWOPERATOR(target->getNickname(), channel->getName()), target->getFd(), target->getNickname());
        sendMsg(RPL_NEWOPERATORFOROWNER(owner->getNickname(), target->getNickname(), channel->getName()), owner->getFd(), owner->getNickname());
        channel->diffuseMsg(RPL_NEWOPERATORFORCHANNEL(target->getNickname(), channel->getName()), clientList, -1);
      }
      else
        sendMsg(RPL_ALREADYOPERATOR(owner->getNickname(), channel->getName(), target->getNickname()), owner->getFd(), owner->getNickname());
  }

  else
  {
    if (channel->FdIsAdmin(target->getFd()))
    {
      channel->removeAdmin(target->getFd());
      sendMsg(RPL_DEOPPED(target->getNickname(), channel->getName()), target->getFd(), target->getNickname());
      sendMsg(RPL_DEOPPEDFOROWNER(owner->getNickname(), target->getNickname(), channel->getName()), owner->getFd(), owner->getNickname());
      channel->diffuseMsg(RPL_DEOPPEDFORCHANNEL(target->getNickname(), channel->getName()), clientList, -1);
    }
    else
      sendMsg(RPL_NOTOPERATOR(owner->getNickname(), channel->getName(), target->getNickname()), owner->getFd(), owner->getNickname());
  }
}

void  tMod(Channel *channel, Client *Target, String mode) {
     
    String msg;
    if (mode == "+t") {
        channel->setTopicPrivilege(true);
        msg = "MODE " + channel->getName() + " +t" + "\r\n";
        send(Target->getFd(), msg.c_str(), msg.size(), 0);
    }
    else if (mode == "-t") {
        channel->setTopicPrivilege(false);
        msg = "MODE " + channel->getName() + " -t" + "\r\n";
        send(Target->getFd(), msg.c_str(), msg.size(), 0);
    }
}
/*
void  kMod(Channel *channel, Client *Target, String mode) {
  
  //  A voir pour parser ça 
  // MODE #chan +k password
}*/

/*
void  lMod(Channel *channel, Client *Target, String mode) {
    
    // MODE #nom-du-canal +l 10
    int limit = 0; // set la limit en fonction du parsing
    if (mode == "+l") {
      channel->setLimitUsers(limit);
      channel->setBoolLimitUsers(true);
    }
    else if (mode == "-l") {
      channel->setLimitUsers(0);
      channel->setBoolLimitUsers(false);
    }
}*/

static void execMode(String mode, Channel *channel, int clientIndex, Client *owner, vector<Client *> clientList, String entry)
{              
  try
  {
    if (mode == "+i" || mode == "-i")                                          
      iMod(channel, owner, mode);             
    else if (mode == "+t" || mode == "-t")                               
      tMod(channel, owner, mode);
    //else if (mode == "+k" || mode == "-k")                    
     // kMod(channel, owner, mode);          
    if (mode == "+o" || mode == "-o")                               
    {
      if (clientIndex == -1)//si le mec a pas rentrer de pseudo cible
        throw (Xception(ERR_UNKNOWNCOMMAND(owner->getNickname(), entry)));
      
      oMod(channel, owner, clientList[clientIndex], mode[0], clientList);  
    }
    //else                              
    //  lMod();
  }

  catch (Xception &e)
  {
    String err = e.what();
    sendMsg(err, owner->getFd(), owner->getNickname());
  }
}

static int  parseModes(vector<String> &modes, String client, String &tmpEntry, String entry, vector<Client *> clientList)
{
  while (tmpEntry.wordCount() != 0)
  {
    String  word = tmpEntry.extractWord(1);
    String  mod;
    char    ope = word[0];

    if (tmpEntry.wordCount() == 0 && ope != '+' && ope != '-')
    { 
      if (Utils::findClientFd(word, clientList) == -1)
        throw (Xception(ERR_NOSUCHNICK(client)));
      return (Utils::findClientFd(word, clientList));
    }

    if (ope != '+' && ope != '-')
      throw (Xception(ERR_UNKNOWNCOMMAND(client, entry)));
    
    for (int j = 0; word[j]; j++)
    {
      if (word[j] == '+' || word[j] == '-')
        ope = word[j];
      if (!Utils::modValidChar(word[j]))
        throw (Xception(ERR_UNKNOWNCOMMAND(client, entry)));//bad input
      else if (word[j] != '+' && word[j] != '-')
      {
        mod.push_back(ope);
        mod.push_back(word[j]);
        modes.push_back(mod);
      }
    }
  }
  return (69); 
}

void  Server::mode(Client *client, String &entry)
{
  vector<String>  modes;
  String          tmpEntry = entry;
  Channel         *channel; 
  int             channelIndex = Utils::findChannelIndex(tmpEntry.extractWord(2), channelList);
  int             fdTarget = -2;
  int             fdClient = client->getFd();
  String          nickClient = client->getNickname();

  tmpEntry.rmWord(1);

  try
  {
    if (channelIndex == -1)//channel existe pas
      throw (Xception(ERR_NOSUCHCHANNEL(nickClient)));
    channel = channelList[channelIndex];

    if (/*!channel->FdIsAdmin(fdClient) && */!channel->FdIsOwner(fdClient))//pas owner
      throw (Xception(ERR_NOTOWNER(nickClient, channel->getName())));

    if (tmpEntry.wordCount() == 0)
      return;

    fdTarget = parseModes(modes, nickClient, tmpEntry, entry, clientList);

    for (int i = 0; i < (int)modes.size(); i++)//execution 1 par 1
      execMode(modes[i], channel, Utils::findClientIndex(fdTarget, clientList), client, clientList, entry);
  }

  catch (Xception &e)
  {
    String err = e.what();
    sendMsg(err, fdClient, nickClient);
  } 
}