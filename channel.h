/*
 * Copyright (C) 2001-2008 Jan Vidar Krey, janvidar@extatic.org
 * See the file "COPYING" for licensing details.
 */

#ifndef HAVE_IRC_CHANNEL_H
#define HAVE_IRC_CHANNEL_H

#include <string>
#include <vector>

namespace IRC
{

class Channel
{
    public:
	Channel(const std::string& name);
	~Channel();

	void join(const std::string& user);
	void part(const std::string& user);
	
    protected:
	std::string name;
	std::string topic;
	std::string modes;
	std::vector<std::string> users;
};


} // namespace IRC

#endif // HAVE_IRC_CHANNEL_H

