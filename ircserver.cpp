/*
 * Copyright (C) 2001-2009 Jan Vidar Krey, janvidar@extatic.org
 * See the file "COPYING" for licensing details.
 */

#include "ircserver.h"
#include "ircconfig.h"
#include "irclink.h"
#include "ircmsg.h"

#include <iostream>
#include <cctype>
#include <cstdlib>

IRC::ServerConnection::ServerConnection(IRC::ServerConfig* cfg, IRC::ServerEventHandler* handler)
{
	socket = 0;
	writeBuffer = 0;
	readBuffer = 0;
	listener = handler;
	config = cfg;
}

IRC::ServerConnection::~ServerConnection()
{
	disconnect(false);
}

void IRC::ServerConnection::connect()
{
	if (!socket)
	{
		std::cout << "Connect..." << std::endl;

		socket = new Samurai::IO::Net::Socket(this, config->getHost(), config->getPort());
		socket->connect();
		writeBuffer = new Samurai::IO::Buffer();
		readBuffer = new Samurai::IO::Buffer();
	}
}

void IRC::ServerConnection::login()
{
	if (config->hasPassword())
	{
		IRC::Message msg_pass("PASS");
		msg_pass.addParameter(config->getPassword());
		send(msg_pass);
	}

	IRC::Message msg_nick("NICK");
	msg_nick.addParameter(config->getNick());
	send(msg_nick);

	IRC::Message msg_user("USER");
	msg_user.addParameter(config->getIdent());
	msg_user.addParameter("8");
	msg_user.addParameter("*");
	msg_user.addParameter(std::string(":") + config->getFullName());
	send(msg_user);

	std::vector<ServerChannelConfig> channels;
	config->getChannels(channels);
	for (std::vector<ServerChannelConfig>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		ServerChannelConfig chancfg = (*it);

		if (chancfg.isAutoJoin())
		{
			IRC::Message msg_join("JOIN");
			msg_join.addParameter(chancfg.getChannel());
			if (chancfg.hasPassword())
			{
				msg_join.addParameter(chancfg.getPassword());
			}
			send(msg_join);
		}
	}
	socket->toggleWriteNotifier(true);
}


void IRC::ServerConnection::disconnect(bool send_event)
{
    	if (send_event)
	{
		listener->OnDisconnected(this);
	}
	
	delete socket; socket = 0;
	delete writeBuffer; writeBuffer = 0;
	delete readBuffer; readBuffer = 0;
}

void IRC::ServerConnection::process()
{
	while (true)
	{
		IRC::Message message;
		bool ok = message.read(readBuffer);
		if (!ok) break;
		
		handle(message);	
	}
}

static void output(const char* msg)
{
	puts(msg);
}

void IRC::ServerConnection::handle(const IRC::Message& msg)
{
	IRC::Message message(msg);
	std::string command = message.getCommand();
	int code = 0;
	if (command.length() == 3 && isdigit(command[0]) && isdigit(command[1]) && isdigit(command[2]))
	{
	    code = std::atoi(command.c_str());
	}
	
	switch (code)
	{
	    case IRC::RPL_WELCOME:
		output("Welcome");
		message.dump();
		break;
		
	    case IRC::RPL_YOURHOST:
	    case IRC::RPL_CREATED:
	    case IRC::RPL_MYINFO:
	    case IRC::RPL_BOUNCE:
		 break;
	    
	    case IRC::RPL_MOTD: /* ignore */
	    case IRC::RPL_LUSERCLIENT:
	    case IRC::RPL_LUSEROP:
	    case IRC::RPL_LUSERUNKNOWN:
	    case IRC::RPL_LUSERCHANNELS:
	    case IRC::RPL_LUSERME:
	    case IRC::RPL_ADMINME:
	    case IRC::RPL_ENDOFNAMES:
	    case IRC::RPL_MOTDSTART:
	    case IRC::RPL_ENDOFMOTD:
		break;
		
	    case IRC::RPL_NAMREPLY:
		output("Got names");
		message.dump();
		break;
	
	    case IRC::RPL_TOPIC:
		output("Got topic");
		message.dump();
		break;

	// undocumented stats stuff
	    case 250:
	    case 265:
	    case 266:
		break;
		
	    case 0:
		if (command == "PING")
		{
			std::string origin = message.getArgument(0, false);
			IRC::Message pong("PONG");
			pong.addParameter(origin);
			send(pong);
			listener->OnPing(this);
			break;
		}
		else if (command == "JOIN")
		{
		    std::string who   = message.getPrefix();
		    std::string what  = message.getArgument(0, true);
		    std::cout << "JOIN: who='" << who << "', what='" << what << "'" << std::endl;
		    break;
		}
		else if (command == "PRIVMSG")
		{
		    std::string who   = message.getPrefix();
		    std::string where = message.getArgument(0, false);
		    std::string what  = message.getArguments(1, true);
		    std::cout << "PRIVMSG: who='" << who << "', what='" << what << "' on='" <<  where << "'" << std::endl;
		    message.dump();
		    break;
		}

	    default:
		message.dump();
		break;

		
	}
	
}

void IRC::ServerConnection::send(const IRC::Message& msg)
{
	IRC::Message message(msg);
	message.write(writeBuffer);
	socket->toggleWriteNotifier(true);
}


void IRC::ServerConnection::EventConnected(const Samurai::IO::Net::Socket*)
{
	std::cout << "Connected" << std::endl;
	listener->OnConnected(this);
	login();
}

void IRC::ServerConnection::EventTimeout(const Samurai::IO::Net::Socket*)
{
	std::cout << "Connect timeout" << std::endl;
	disconnect();
}

void IRC::ServerConnection::EventDisconnected(const Samurai::IO::Net::Socket*)
{
	disconnect();
}

void IRC::ServerConnection::EventDataAvailable(const Samurai::IO::Net::Socket*)
{
	size_t bytes = socket->getReceiveBufferSize();
	char* buffer = new char[bytes+1];
	ssize_t ret  = socket->read(buffer, bytes);
	if (ret > 0)
	{
		readBuffer->append(buffer, ret);
	}
	delete[] buffer;

	process();
}

void IRC::ServerConnection::EventCanWrite(const Samurai::IO::Net::Socket*)
{
	size_t remaining = writeBuffer->size();

	if (remaining)
	{
		char* buffer = new char[remaining];
		writeBuffer->pop(buffer, remaining);
		
		ssize_t ret = socket->write(buffer, remaining);
		delete[] buffer;
		if (ret >= 0)
		{
			writeBuffer->remove(ret);
			remaining -= ret;
		}
		else
		{
			// ERROR!
			return;
		}
	}

	socket->toggleWriteNotifier(remaining > 0);
}

void IRC::ServerConnection::EventError(const Samurai::IO::Net::Socket*, enum Samurai::IO::Net::SocketError error, const char* msg)
{
	(void) error;
	(void) msg;
	disconnect();
}
