#ifndef EUKIT_INI_BASE
#define EUKIT_INI_BASE

#include <string>

namespace EUkit {
namespace ini {
enum class Type { INT, STR };
struct Value {
	int Int;
	std::string Str;
};
union CheckFunc {
	bool (*Int)(int);
	bool (*Str)(const std::string &);
};
class Field {
public:
	std::string section;
	std::string keyname;
	Field();
	Field(const std::string &section, const std::string &keyname);
	bool operator<(const Field &right) const;
};

class Element {
public:
	Element();
	Element(Type type, const std::string &section, const std::string &keyname,
			const Value &default_value, CheckFunc check_func, bool is_exist,
			const Value &real_valuel);
	void set(Type type, const std::string &section, const std::string &keyname,
			 const Value &default_value, CheckFunc check_func, bool is_exist,
			 const Value &real_valuel);
	Type type;
	std::string section;
	std::string keyname;
	Value default_value;
	CheckFunc check_func;
	bool is_exist;
	Value real_value;
};

// return true if error
bool _defaultParser(const std::string &textline, std::string &section,
					std::string &keyname, std::string &value);
} // namespace ini
} // namespace EUkit
#endif