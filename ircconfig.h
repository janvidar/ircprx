#ifndef HAVE_IRC_CONFIG_H
#define HAVE_IRC_CONFIG_H

#include <string>
#include <vector>

namespace IRC
{

class ServerChannelConfig
{
	public:
		ServerChannelConfig(const std::string& channel, const std::string& password = "", bool autojoin = true);
		virtual ~ServerChannelConfig();
		
		std::string getChannel() const { return channel; }
		std::string getPassword() const { return password; }
		bool hasPassword() const { return password != ""; }
		bool isAutoJoin() const { return autojoin; }

	protected:
		std::string channel;
		std::string password;
		bool autojoin;
};


class ServerConfig
{
	public:
		ServerConfig(const std::string& address, uint16_t port = 6667, bool ssl = false, const std::string& password = "");
		void setIdentity(const std::string& nick, const std::string& altnick, const std::string& ident, const std::string& fullname);
		void addChannel(const std::string& channel, bool autojoin = true, const std::string& password = "");

		std::string getHost() const { return address; }
		uint16_t getPort() const { return port; }

		std::string getIdent() const { return ident; }
		std::string getNick() const { return nick; }
		std::string getAltNick() const { return altnick; }
		std::string getAwayNick() const { return awaynick; }
		std::string getFullName() const { return fullname; }
		std::string getPassword() const { return password; }

		bool hasPassword() const { return password != ""; }

		void getChannels(std::vector<ServerChannelConfig>& chans);

	protected:
		std::string address;
		uint16_t    port;
		bool ssl;
		std::string password;
		bool autojoin;
		std::string nick;
		std::string altnick;
		std::string awaynick;
		std::string ident;
		std::string fullname;
		std::vector<ServerChannelConfig> channels;
};

class LocalConfig
{
	public:
		LocalConfig();
		virtual ~LocalConfig();

	protected:
		uint16_t port;
		// TODO: SSL stuff
};

class Config
{
	public:
		Config();
		virtual ~Config();
		
		bool load();
		bool save();
		
	protected:
		std::vector<ServerConfig> servers;
		LocalConfig local;
};

}

#endif // HAVE_IRC_CONFIG_H
