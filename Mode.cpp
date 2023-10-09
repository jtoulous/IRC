#include "Server.hpp"                                                      

void    iMod(Channel *channel, Client *Target, String mode);
void    oMod(Channel *channel, Client *owner, String targetNick, char operation, vector<Client *> clientList);
void    tMod(Channel *channel, Client *Target, String mode);
void    kMod(String mode, Channel *channel, String arg, Client*owner);
void    lMod(String mode, Channel *channel, String arg, Client*owner);

static void execMode(String mode, Channel *channel, String argsList, Client *owner, vector<Client *> clientList)
{              
  String arg;

  try
  {
    if (mode == "+i" || mode == "-i")                                          
      iMod(channel, owner, mode);             
    else if (mode == "+t" || mode == "-t")                               
      tMod(channel, owner, mode);
    
    else
    {
      arg = argsList.extractWord(1);
      
      if (mode == "+o" || mode == "-o")                               
        oMod(channel, owner, arg, mode[0], clientList);  
      else if (mode == "+k" || mode == "-k")                    
        kMod(mode, channel, arg, owner);          
      else                              
        lMod(mode, channel, arg, owner);
    }
  }

  catch (Xception &e)
  {
    String err = e.what();
    sendMsg(err, owner->getFd(), owner->getNickname());
  }
}

static int  parseModes(vector<String> &modes, String client, String &tmpEntry, String entry)
{
  int     nbArgs = 0;
  
  while (tmpEntry.getWord(1)[0] == '+' || tmpEntry.getWord(1)[0] == '-')
  {
    String  word = tmpEntry.extractWord(1);
    String  mod;
    char    ope = word[0];
    
    for (int j = 1; word[j]; j++)
    {
      if (word[j] == '+' || word[j] == '-')
        ope = word[j];
      
      if (!Utils::modValidChar(word[j]))
        throw (Xception(ERR_UNKNOWNCOMMAND(client, entry)));//bad input
      
      if (word[j] == 'o' || word[j] == 'l' || word[j] == 'k')
        nbArgs++;
      
      if (word[j] != '+' && word[j] != '-')
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
  int             fdClient = client->getFd();
  String          nickClient = client->getNickname();
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
    

    nbArgs = parseModes(modes, nickClient, tmpEntry, entry);

    if (nbArgs != tmpEntry.wordCount())
      throw (Xception(ERR_UNKNOWNCOMMAND(nickClient, entry)));

    for (int i = 0; i < (int)modes.size(); i++)//execution 1 par 1
      execMode(modes[i], channel, tmpEntry, client, clientList);
  }

  catch (Xception &e)
  {
    String err = e.what();
    sendMsg(err, fdClient, nickClient);
  } 
}