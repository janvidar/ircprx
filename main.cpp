/*
 * Copyright (C) 2001-2008 Jan Vidar Krey, janvidar@extatic.org
 * See the file "COPYING" for licensing details.
 */

#include <samurai/samurai.h>
#include <samurai/io/net/socketmonitor.h>

#include <iostream>
#include <string>
#include <vector>

#include "irclink.h"
#include "ircserver.h"
#include "ircconfig.h"

class EventHandler : public IRC::ServerEventHandler
{
public:

protected:
	void OnConnected(IRC::ServerConnection*)  { }
	void OnDisconnected(IRC::ServerConnection*) { }
	void OnNickError(IRC::ServerConnection*)  { }
	void OnMessage(IRC::ServerConnection*)    { }
	void OnPing(IRC::ServerConnection*)       { }
	void OnRawMessage(IRC::ServerConnection*) { }
};

void run_tests()
{
    IRC::Message m1(":hordak!hordak@is.an.irc-pimp.org PRIVMSG #fw :why not");
    IRC::Message m2(":xero_art!Mibbit@168.223.203.105 QUIT :http://www.mibbit.com ajax IRC Client");
    
    std::cout << "test 1 " << m1.getArgument(0, false) << std::endl;
    std::cout << "test 2 " << m1.getArgument(1, false) << std::endl;
    std::cout << "test 3 " << m1.getArgument(2, false) << std::endl;
    std::cout << "test 4 " << m1.getArguments(1, true) << std::endl;
    
}

int main(int argc, char** argv)
{
	(void) argc;
	(void) argv;
	bool running = true;
	Samurai::IO::Net::SocketMonitor* monitor = Samurai::IO::Net::SocketMonitor::getInstance();
	
	std::cout << "ircprx" << std::endl;

	IRC::ServerConfig serverConfig("irc.efnet.org");
	serverConfig.setIdentity("qopiaz", "qopiaz__", "jalla", "Tester Jalla");
	serverConfig.addChannel("#ircprx");
	serverConfig.addChannel("#fw");

	EventHandler handler;
	IRC::ServerConnection connection(&serverConfig, &handler);
	connection.connect();

	while (running)
	{
		monitor->wait(5000);
	}
}

