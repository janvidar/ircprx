/*
 * Copyright (C) 2001-2009 Jan Vidar Krey, janvidar@extatic.org
 * See the file "COPYING" for licensing details.
 */

class Identity
{
	public:
		Identity(const std::string& nick, const std::string& ident, const std::string& fullname);
		virtual ~Identity();

	protected:
		std::string nick;
		std::string ident;
		std::string fullname;
};


