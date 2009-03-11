/*
 * Copyright (C) 2001-2009 Jan Vidar Krey, janvidar@extatic.org
 * See the file "COPYING" for licensing details.
 */

#ifndef HAVE_IRC_LINK_H
#define HAVE_IRC_LINK_H

#include <samurai/samurai.h>
#include <samurai/io/net/socket.h>
#include <samurai/io/net/datagram.h>
#include <samurai/io/net/serversocket.h>
#include <samurai/io/net/socketevent.h>
#include <samurai/io/net/socketmonitor.h>
#include <samurai/io/buffer.h>

namespace IRC
{

class ServerConnection;
class Message;

/**
 * Common interface for handling messages to/from IRC servers.
 */
class ServerEventHandler
{
public:
	/**
	We are connected to the server
	*/
	virtual void OnConnected(ServerConnection*) = 0;
	
	/**
	We are disconnected from the server
	*/
	virtual void OnDisconnected(ServerConnection*) = 0;
	
	/**
	 * Nick name error - try an alternate nick name,
	 * or, consider cycling IRC servres with a delay
	 * to obtain the desired nick name.
	 * - The more nick names available, the more tries
	 * we can do without problems.
	 */
	virtual void OnNickError(ServerConnection*) = 0;
	
	/**
	 * A "message" occured (either, chat, privmsg, join/leave, or whatever)
	 * For detached mode we would like to store the message until we actually
	 * attach an IRC client.
	 */
	virtual void OnMessage(ServerConnection*) = 0;
	
	/**
	 * We got a ping, and must pong the server, otherwise the
	 * server will disconnect us.
	 * We don't bother to forward this to any attached clients
	 * as this is the proxy's business, really.
	 */
	virtual void OnPing(ServerConnection*) = 0;
	
	/**
	 * Handle any "raw" message from the irc server.
	 * Sort of a "catch all" for an imperfect interface :-)
	 */
	virtual void OnRawMessage(ServerConnection*) = 0;
};


/**
 * When a user (or IRC client) is attached to the proxy,
 * handle the messages by forwarding messages to the server.
 *
 * PING message is always handled locally.
 */
class AttachedHandler : public ServerEventHandler
{
	public:
		void OnStatus();
		void OnDisconnected();
		void OnNickError();
		void OnMessage();
		void OnPing();
};

/**
 * When a user (or IRC client) is NOT attached to the proxy,
 * handle the messages automatically.
 */
class DetachedHandler : public ServerEventHandler
{
	public:
		void OnConnected();
		void OnDisconnected();
		void OnNickError();
		void OnMessage();
		void OnPing();
};


class Link
{
	public:
		Link();
		virtual ~Link();
		
		void addServer(ServerConnection* connection);
	
	protected:
		
};

} // namespace IRC

#endif // HAVE_IRC_SERVER_CONNECTION_H

