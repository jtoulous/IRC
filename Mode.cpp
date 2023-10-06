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

static void execMode(String mode, Channel *channel, int clientIndex, Client *owner, vector<Client *> clientList, String entry)
{              
  try
  {
    //if (mode == "+i" || mode == "-i")                                          
    //  iMod();             
    //else if (mode == "+t" || mode == "-t")                               
    //  tMod();
    //else if (mode == "+k" || mode == "-k")                       
    //  kMod();          
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

static void  parseModes(vector<String> &modes, String client, String tmpEntry, String entry)
{
  char    ope = tmpEntry.wordStartChar(1);
  String  mod;

  if (ope != '-' && ope != '+')
        throw (Xception(ERR_UNKNOWNCOMMAND(client, entry)));//bad input
  
  for (int i = 1; i <= tmpEntry.wordCount(); i++)
  {
    String word = tmpEntry.getWord(i);

    for (int j = 0; word[j]; j++)
    {
      if (!Utils::modValidChar(word[j]))
        throw (Xception(ERR_UNKNOWNCOMMAND(client, entry)));//bad input
      
      if (word[j] == '+' || word[j] == '-')
      {
        if (word[j + 1] == '+' || word[j + 1] == '-')
          throw (Xception(ERR_UNKNOWNCOMMAND(client, entry)));//bad input
        ope = word[j];
      }

      else
      {
        mod.push_back(ope);
        mod.push_back(word[j]);
        modes.push_back(mod);
        mod.clear();
      }
    }  
  }
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

    if (tmpEntry.wordStartChar(1) != '+' && tmpEntry.wordStartChar(1) != '-')
    { 
      if (tmpEntry.wordCount() == 1)
        throw (Xception(ERR_UNKNOWNCOMMAND(nickClient, entry)));
      fdTarget = Utils::findClientFd(tmpEntry.extractWord(1), clientList);
      if (fdTarget == -1)//le client cible existe pas
        throw (Xception(ERR_NOSUCHNICK(nickClient)));
    }

    parseModes(modes, nickClient, tmpEntry, entry);

    for (int i = 0; i < (int)modes.size(); i++)//execution 1 par 1
      execMode(modes[i], channel, Utils::findClientIndex(fdTarget, clientList), client, clientList, entry);
  }

  catch (Xception &e)
  {
    String err = e.what();
    sendMsg(err, fdClient, nickClient);
  } 
}