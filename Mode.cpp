#include "Server.hpp"                                                      

static bool isMod(String word)
{
  if (word[0] == '-' || word[0] == '+')
    return true;
  return false;
}

static void getArgs(String entry, vector<String> &args)
{
  while (entry.wordCount() != 0)
    args.push_back(entry.extractWord(1));
}

static void oMod(Channel *channel, Client *owner, String targetNick, char operation, vector<Client *> clientList)
{
  int     fdTarget = Utils::findClientFd(targetNick, clientList);
  Client  *target;
  
  if (fdTarget == -1)
    throw (Xception(ERR_NOSUCHNICK(targetNick)));
  
  target = clientList[Utils::findClientIndex(fdTarget, clientList)];
  
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

static void execMode(String mode, Channel *channel, vector<String> argsList, Client *owner, vector<Client *> clientList, String entry)
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
      if (arg.empty())
        throw (Xception(ERR_UNKNOWNCOMMAND(owner->getNickname(), entry)));
      oMod(channel, owner, arg, mode[0], clientList);  
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

static void  parseModes(vector<String> &modes, String client, String &tmpEntry, String entry)
{
  while (isMod(tmpEntry.getWord(1)))
  {
    String  word = tmpEntry.extractWord(1);
    String  mod;
    char    ope = word[0];
    int     nbArgs = 0;

    //if (tmpEntry.wordCount() == 0 && ope != '+' && ope != '-')
    //  return;

    //if (ope != '+' && ope != '-')
    //  throw (Xception(ERR_UNKNOWNCOMMAND(client, entry)));
    
    for (int j = 1; word[j]; j++)
    {
      if (word[j] == '+' || word[j] == '-')
        ope = word[j];
      if (!Utils::modValidChar(word[j]))
        throw (Xception(ERR_UNKNOWNCOMMAND(client, entry)));//bad input
      if (word[j] == 'o' || word[j] == 'l' || word[j] == 'k')
        nbArgs++;
      else if (word[j] != '+' && word[j] != '-')
      {
        mod.push_back(ope);
        mod.push_back(word[j]);
        modes.push_back(mod);
      }
    }
  }
  return (nbArgs);
}

void  Server::mode(Client *client, String &entry)
{
  vector<String>  modes;
  String          tmpEntry = entry;
  Channel         *channel; 
  int             channelIndex = Utils::findChannelIndex(tmpEntry.extractWord(2), channelList);
  //int             fdTarget = -2;
  int             fdClient = client->getFd();
  String          nickClient = client->getNickname();
  vector<String>          argsList;
  int             nbMod;
  int             nbArgs;

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
    
0
    nbArgs = parseModes(modes, nickClient, tmpEntry, entry);
    getArgs(tmpEntry, argsList);

    if (nbArgs != argsList.size())
      throw (Xception(ERR_UNKNOWNCOMMAND(client, entry)));

    for (int i = 0; i < (int)modes.size(); i++)//execution 1 par 1
      execMode(modes[i], channel, argsList, client, clientList, entry);
  }

  catch (Xception &e)
  {
    String err = e.what();
    sendMsg(err, fdClient, nickClient);
  } 
}