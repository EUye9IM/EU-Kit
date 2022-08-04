#ifndef EUKIT_INI_INI
#define EUKIT_INI_INI

#include <ini/base.h>
#include <istream>
#include <map>

namespace EUkit {
namespace ini {
class Parser {
public:
	Parser();
	Parser &setParseRule(bool (*parseRule)(const std::string &, std::string &,
										   std::string &, std::string &));
	Parser &addInt(const std::string &section, const std::string &keyname,
				   int default_value = 0, bool (*check_func)(int) = nullptr);
	Parser &addString(const std::string &section, const std::string &keyname,
					  const std::string &default_value = "",
					  bool (*check_func)(const std::string &) = nullptr);
	Parser &bindInt(const std::string &section, const std::string &keyname,
					int &value, bool (*check_func)(int) = nullptr);
	Parser &bindString(const std::string &section, const std::string &keyname,
					   std::string &value,
					   bool (*check_func)(const std::string &) = nullptr);

	bool parseFile(const char *file_path);
	bool success() const;
	const char *errmsg() const;

	int getInt(const std::string &section, const std::string &keyname) const;
	const char *getString(const std::string &section,
						  const std::string &keyname) const;

private:
	void _add(Type type, const std::string &section, const std::string &keyname,
			  const Value &default_value, CheckFunc check_func);
	void _parse(std::istream &);
	bool (*_parseRule)(const std::string &, std::string &, std::string &,
					   std::string &);
	bool _is_sucess;
	std::string _log;
	std::map<Field, Element> _elements;

	std::map<Field, int *> _bind_int;
	std::map<Field, std::string *> _bind_string;
};
} // namespace ini
} // namespace EUkit
#endif