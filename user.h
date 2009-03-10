/*
 * Copyright (C) 2001-2008 Jan Vidar Krey, janvidar@extatic.org
 * See the file "COPYING" for licensing details.
 */

#ifndef HAVE_IRC_USERL_H
#define HAVE_IRC_USERL_H

#include <string>
#include <vector>

namespace IRC
{

class User
{
    public:
	User(const std::string& name);
	~User();

	void setData(const std::string& data);
	void setNick(const std::string& nick);
	
	std::string getNick();
	std::string getIdent();
	std::string getHost();
	
	bool operator==(const User&);
	bool operator==(const std::string&);

    protected:
	std::string data;
	int flags;
};


} // namespace IRC

#endif // HAVE_IRC_USERL_H

