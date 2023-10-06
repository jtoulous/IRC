#include "Server.hpp"                                                      

static void execMode(String mode, Channel *channel, Client *Target, Client *Admin)
{                                                                            
  //if (mode == '+i' || mode == '-i')                                          
  //  iMod();              
  //else if (mode == '+t' || mode == '-t')                               
  //  tMod();             
  //else if (mode == '+k' || mode == '-k')                                 
  //  kMod();          
  //else if (mode == '+o' || mode == '-o')                               
  //  oMod();          
  //else if (mode == '+l' || mode == '-l')                                
  //  lMod();
  //else
    sendMsg(ERR_NOTVALIDMOD(), Admin->getFd(), Admin->getNickname());
}

static void  parseModes(vector<String> &modes, String entry, String client)
{
  char    ope = entry.wordStartChar(1);
  String  mod;

  if (ope != '-' && ope != '+')
        throw (Xception(ERR_UNKNOWNCOMMAND(client, entry)));//bad input
  
  for (int i = 1; i <= entry.wordCount(); i++)
  {
    String word = entry.getWord(i);

    for (int j = 0; word[j]; j++)
    {
      if (!Utils::modValidChar(word[j]))
        throw (Xception(ERR_UNKNOWNCOMMAND(client, entry)));//bad input
      
      if (word[j] == '+' || word[j] == '-')
        ope = word[j];
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
  Channel         *channel; 
  int             channelIndex = Utils::findChannelIndex(entry.extractWord(2), channelList);
  int             fdTarget = -2;
  int             fdClient = client->getFd();
  String          nickClient = client->getNickname();

  entry.rmWord(1);

  try
  {
    if (channelIndex == -1)//channel existe pas
      throw (Xception(ERR_NOSUCHCHANNEL(nickClient)));
    channel = channelList[channelIndex];

    if (!channel->FdIsAdmin(fdClient) && !channel->FdIsOwner(fdClient))//pas admin
      throw (Xception(ERR_CHANOPRIVSNEEDED(nickClient, channel->getName())));

    if (entry.wordCount() == 0)
      return;

    if (entry.wordStartChar(1) != '+' && entry.wordStartChar(1) != '-')
    { 
      if (entry.wordCount() == 1)
        throw (Xception(ERR_UNKNOWNCOMMAND(nickClient, entry)));
      fdTarget = Utils::findClientFd(entry.extractWord(1), clientList);
      if (fdTarget == -1)//le client cible existe pas
        throw (Xception(ERR_NOSUCHNICK(nickClient)));
    }

    parseModes(modes, entry, nickClient);

    for (int i = 0; i < (int)modes.size(); i++)//execution 1 par 1
      execMode(modes[i], channel, clientList[Utils::findClientIndex(fdTarget)], client);
  }

  catch (Xception &e)
  {
    String err = e.what();
    sendMsg(err, fdClient, nickClient);
  } 
}