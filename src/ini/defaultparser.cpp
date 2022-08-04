#include <ini/ini.h>
#include <regex>

using std::regex;
using std::regex_match;
using std::smatch;
using std::string;

namespace EUkit::ini {
// return true if success
bool _defaultParser(const std::string &textline, std::string &section,
					std::string &keyname, std::string &value) {
	string sectionPattern = "(?:\\[(?:\\s*)([^\\]\\s]+)(?:\\s*)\\])";
	string kvPattern = "(?:([^\\s=#;]+)(?:\\s*)=(?:\\s*)([^\\s#;]+))";
	string pattern = "^(?:\\s*)(?:" + sectionPattern + "|" + kvPattern +
					 ")?(?:\\s*)(?:(?:;|#).*)?$";

	regex re(pattern);
	smatch m;
	bool ret;
	ret = regex_match(textline, m, re);
	string s = m[1].str();
	keyname = m[2].str();
	value = m[3].str();
	if (s != "") {
		section = s;
	}
	return ret;
}
} // namespace EUkit::ini
