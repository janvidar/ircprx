/*
 * Copyright (C) 2001-2009 Jan Vidar Krey, janvidar@extatic.org
 * See the file "COPYING" for licensing details.
 */

#include "ircmsg.h"
#include <iostream>
#include <sstream>

// #define IRC_MSG_DEBUG
// #define IRC_MSG_DEBUG_SPLIT

IRC::Message::Message()
{
}

IRC::Message::Message(const IRC::Message& msg)
{
	message = msg.message;
	parse();
}

IRC::Message::Message(const std::string& command)
{
	message = command;
	parse();
}

IRC::Message::~Message()
{
}

void IRC::Message::addParameter(const std::string& param)
{
    	params.push_back(param);
	message += " ";
	message += param;
}

void IRC::Message::write(Samurai::IO::Buffer* buffer)
{
	buffer->append(message);
	buffer->append("\r\n");
#ifdef IRC_MSG_DEBUG
	std::cout << "SEND: " << message << std::endl;
#endif
}

bool IRC::Message::read(Samurai::IO::Buffer* buffer)
{
	int offset = buffer->find('\n');
	if (offset == -1)
		return false;
	
	message = buffer->pop(offset);
	buffer->remove(offset+1);
	
	// strip 'cr'
	if (message[message.size()-1] == '\r')
		message = message.substr(0, message.size()-1);

#ifdef IRC_MSG_DEBUG
	std::cout << "READ: " << message << std::endl;
#endif
	return true;
}

std::string IRC::Message::getCommand()
{
    return command;
}

std::string IRC::Message::getPrefix()
{
    return prefix;
}

std::string IRC::Message::getArgument(size_t offset, bool stripColon)
{
    if (offset >= params.size())
	throw "Out of bounds!";
    
    std::string tmp = params[offset];
    if (stripColon && tmp.length() >= 1 && tmp[0] == ':')
	    tmp = tmp.substr(1);
    return tmp;
}

std::string IRC::Message::getArguments(size_t offset, bool stripColon)
{
    if (params.size() == 0 || offset >= params.size())
	return "";
    
    std::string tmp = params[offset];
    if (stripColon && tmp.length() >= 1 && tmp[0] == ':')
	    tmp = tmp.substr(1);
    for (size_t n = offset + 1; n < params.size(); n++)
    {
	tmp += " ";
	tmp += params[n];
    }
    return tmp;
}

void IRC::Message::parse()
{
	size_t offset = 0;
	size_t length = 0;
	
	prefix = "";
	command = "";
	
	if (message[0] == ':')
	{
		offset = message.find(' ');
		if (offset == std::string::npos)
		{
			throw "Wtf?";
		}
		else
		{
		    prefix = message.substr(1, offset-1);
		    offset++;
		}
		
	}
	
	length = message.find(' ', offset);
	if (length == std::string::npos)
	{
		command = message.substr(offset);
	}
	else
	{
		
		length -= offset;
		command = message.substr(offset, length);
		
		// extract parameters and push them into the params vector
		offset += length;
		offset++;
		length = message.find(' ', offset);
		while (length != std::string::npos)
		{
		    length -= offset;
		    std::string arg = message.substr(offset, length);
#ifdef IRC_MSG_DEBUG_SPLIT
		    std::cout << "ARG: arg[" << params.size() << "]='" << arg << std::endl;
#endif

		    params.push_back(arg);

		    offset += length;
		    offset ++;
		    length = message.find(' ', offset);
		}
		
		if (message.size() > offset)
		{
		    params.push_back(message.substr(offset));
		}
	}

#ifdef IRC_MSG_DEBUG_SPLIT
	std::cout << "DETAIL: prefix='" << prefix  << "', command='" << command << "', arguments='" << params.size() << "'" << std::endl;
#endif
}

void IRC::Message::dump()
{
	std::cout << "DUMP: '" << message << "'" << std::endl;
	std::cout << "DETAIL: prefix='" << prefix  << "', command='" << command << "', arguments='" << params.size() << "'" << std::endl;
}
