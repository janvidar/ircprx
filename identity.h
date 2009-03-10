
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


