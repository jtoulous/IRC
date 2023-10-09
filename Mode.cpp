#include "Server.hpp"                                                      

static void execMode(String mode, Channel *channel, String arg, Client *owner, vector<Client *> clientList, String entry)
{              
  try
  {
    if (mode == "+i" || mode == "-i")                                          
      iMod(channel, owner, mode);             
    else if (mode == "+t" || mode == "-t")                               
      tMod(channel, owner, mode);
    else if (mode == "+k" || mode == "-k")                    
      kMod(mode, channel, arg, owner);        
    else if (mode == "+o" || mode == "-o")                               
    {  
      if (arg.empty())
        throw (Xception(ERR_UNKNOWNCOMMAND(owner->getNickname(), entry)));
      oMod(channel, owner, arg, mode[0], clientList);  
    }
    else if (mode == "+l" || mode == "-l")                              
      lMod(mode, channel, arg, owner);
  }

  catch (Xception &e)
  {
    String err = e.what();
    sendMsg(err, owner->getFd(), owner->getNickname());
  }
}

static void  parseModes(vector<String> &modes, String client, String &tmpEntry, String entry)
{
  while (tmpEntry.wordCount() != 0)
  {
    String  word = tmpEntry.extractWord(1);
    String  mod;
    char    ope = word[0];

    if (tmpEntry.wordCount() == 0 && ope != '+' && ope != '-')
      return;

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
  String          arg;

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

    parseModes(modes, nickClient, tmpEntry, entry);

    if (entry.lastWord()[0] != '-' && entry.lastWord()[0] != '+')
      arg = entry.lastWord();

    for (int i = 0; i < (int)modes.size(); i++)//execution 1 par 1
      execMode(modes[i], channel, arg, client, clientList, entry);
  }

  catch (Xception &e)
  {
    String err = e.what();
    sendMsg(err, fdClient, nickClient);
  } 
}