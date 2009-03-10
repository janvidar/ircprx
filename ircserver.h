/*
 * Copyright (C) 2001-2008 Jan Vidar Krey, janvidar@extatic.org
 * See the file "COPYING" for licensing details.
 */

#ifndef HAVE_IRC_SERVER_CONNECTION_H
#define HAVE_IRC_SERVER_CONNECTION_H

#include <samurai/samurai.h>
#include <samurai/io/net/socket.h>
#include <samurai/io/net/socketevent.h>
#include <samurai/io/buffer.h>

#include "ircmsg.h"

namespace IRC
{

class ServerConfig;
class ServerEventHandler;

class ServerConnection : public Samurai::IO::Net::SocketEventHandler
{
	public:
		ServerConnection(ServerConfig*, ServerEventHandler*);
		virtual ~ServerConnection();
		
		void connect();
		void disconnect(bool send_event = true);
	
	protected:
		/**
		 * Process the readbuffer.
		 */
		void process();
		void login();
		void ping();
		void handle(const IRC::Message& message);
		void send(const IRC::Message& message);

	protected:
		void EventConnected(const Samurai::IO::Net::Socket*);
		void EventDisconnected(const Samurai::IO::Net::Socket*);
		void EventDataAvailable(const Samurai::IO::Net::Socket*);
		void EventCanWrite(const Samurai::IO::Net::Socket*);
		void EventError(const Samurai::IO::Net::Socket*, enum Samurai::IO::Net::SocketError error, const char* msg);
		void EventTimeout(const Samurai::IO::Net::Socket*);
		
	protected:
		Samurai::IO::Net::Socket* socket;
		Samurai::IO::Buffer* writeBuffer;
		Samurai::IO::Buffer* readBuffer;
		ServerEventHandler* listener;
		ServerConfig* config;
};

} // namespace IRC

#endif // HAVE_IRC_SERVER_CONNECTION_H

