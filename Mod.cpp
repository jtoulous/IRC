#include "Server.hpp"


/*  MODE #Finnish +i  */
void    iMod(Channel *channel, Client *Target, String mode) {

    String msg;
    if (mode == "+i") {
        channel->setBoolInviteOnly(true);
        msg = "MODE " + channel->getName() + " +i" + "\r\n";
        send(Target->getFd(), msg.c_str(), msg.size(), 0);
    }
    else if (mode == "-i") {
        channel->setBoolInviteOnly(false);
        msg = "MODE " + channel->getName() + " -i" + "\r\n";
        send(Target->getFd(), msg.c_str(), msg.size(), 0);
    }
}

void oMod(Channel *channel, Client *owner, String targetNick, char operation, vector<Client *> clientList)
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

void  kMod(String mode, Channel *channel, String arg, Client*owner) {
  
  String  msg;

  if (mode == "+k") {
        channel->setPassword(arg);
        msg = "MODE " + channel->getName() + " +k" + "\r\n";
        send(owner->getFd(), msg.c_str(), msg.size(), 0);
  }
  else if (mode == "-k") {
        channel->setPassword("");
        msg = "MODE " + channel->getName() + " -k" + "\r\n";
        send(owner->getFd(), msg.c_str(), msg.size(), 0);
  }
}

void  lMod(String mode, Channel *channel, String arg, Client*owner) {
    
  int limit = atoi(arg.c_str());
  String  msg;

  if (mode == "+l") {
          
          channel->setLimitUsers(limit); 
          channel->setBoolLimitUsers(true);
          msg = "MODE " + channel->getName() + " +l" + "\r\n";
          send(owner->getFd(), msg.c_str(), msg.size(), 0);
  }
  else if (mode == "-l") {
        channel->setLimitUsers(0);
        channel->setBoolLimitUsers(false);
        msg = "MODE " + channel->getName() + " -l" + "\r\n";
        send(owner->getFd(), msg.c_str(), msg.size(), 0);
  }
}