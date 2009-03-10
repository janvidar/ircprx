#include "ircconfig.h"


IRC::ServerChannelConfig::ServerChannelConfig(const std::string& chan, const std::string& pwd, bool aj) : channel(chan), password(pwd), autojoin(aj)
{

}
IRC::ServerChannelConfig::~ServerChannelConfig()
{
}

IRC::ServerConfig::ServerConfig(const std::string& adr, uint16_t p, bool ssl_, const std::string& pwd) : address(adr), port(p), ssl(ssl_), password(pwd) 
{
	
}

void IRC::ServerConfig::setIdentity(const std::string& nick_, const std::string& altnick_, const std::string& ident_, const std::string& fullname_)
{
	nick = nick_;
	altnick = altnick_;
	ident = ident_;
	fullname = fullname_;
}

void IRC::ServerConfig::addChannel(const std::string& channel, bool autojoin, const std::string& pwd)
{
	ServerChannelConfig chan(channel, pwd, autojoin);
	channels.push_back(chan);
}

void IRC::ServerConfig::getChannels(std::vector<ServerChannelConfig>& chans)
{
	std::vector<ServerChannelConfig>::iterator it;
	for (it = channels.begin(); it != channels.end(); it++)
	{
		chans.push_back(*it);
	}
}
