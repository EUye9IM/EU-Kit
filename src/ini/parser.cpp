#include <fstream>
#include <ini/ini.h>

using std::string;
using std::to_string;

namespace EUkit {
namespace ini {
Parser::Parser() {
	_is_sucess = false;
	this->setParseRule(_defaultParser);
}
Parser &Parser::setParseRule(bool (*parseRule)(const std::string &,
											   std::string &, std::string &,
											   std::string &)) {
	this->_parseRule = parseRule;
	return *this;
}
Parser &Parser::addInt(const std::string &section, const std::string &keyname,
					   int default_value, bool (*check_func)(int)) {
	Value v;
	v.Int = default_value;
	this->_add(Type::INT, section, keyname, v, CheckFunc{.Int = check_func});
	return *this;
}
Parser &Parser::addString(const std::string &section,
						  const std::string &keyname,
						  const std::string &default_value,
						  bool (*check_func)(const std::string &)) {
	Value v;
	v.Str = default_value;
	this->_add(Type::STR, section, keyname, v, CheckFunc{.Str = check_func});
	return *this;
}
Parser &Parser::bindInt(const std::string &section, const std::string &keyname,
						int &value, bool (*check_func)(int)) {
	Value v;
	v.Int = value;
	this->_add(Type::INT, section, keyname, v, CheckFunc{.Int = check_func});
	this->_bind_int[Field(section, keyname)] = &value;
	return *this;
}
Parser &Parser::bindString(const std::string &section,
						   const std::string &keyname, std::string &value,
						   bool (*check_func)(const std::string &)) {
	Value v;
	v.Str = value;
	this->_add(Type::STR, section, keyname, v, CheckFunc{.Str = check_func});
	this->_bind_string[Field(section, keyname)] = &value;
	return *this;
}
bool Parser::parseFile(const char *file_path) {
	std::ifstream ifs(file_path);
	if (!(ifs.good() && ifs.is_open())) {
		_log +=
			"Error: Openning file '" + std::string(file_path) + "' failed.\n";
		_is_sucess = false;
		return false;
	}
	this->_parse(ifs);
	ifs.close();
	return _is_sucess;
}
bool Parser::success() const { return _is_sucess; }
const char *Parser::errmsg() const { return _log.c_str(); }
int Parser::getInt(const std::string &section,
				   const std::string &keyname) const {
	Field key(section, keyname);
	try {
		return _elements.at(key).real_value.Int;
	} catch (...) {
	}

	return 0;
}
const char *Parser::getString(const std::string &section,
							  const std::string &keyname) const {
	Field key(section, keyname);
	try {
		return _elements.at(key).real_value.Str.c_str();
	} catch (...) {
	}

	return "";
}
void Parser::_add(Type type, const std::string &section,
				  const std::string &keyname, const Value &default_value,
				  CheckFunc check_func) {
	Field field(section, keyname);
	_elements[field].set(type, section, keyname, default_value, check_func,
						 false, default_value);
}

void Parser::_parse(std::istream &in) {
	// init
	_is_sucess = true;
	_log = "";
	for (auto e = _elements.begin(); e != _elements.end(); e++) {
		e->second.is_exist = false;
		e->second.real_value = e->second.default_value;
	}

	// start parse
	string section = "";
	int lineCnt = 0;
	while (!in.eof()) {
		lineCnt++;
		string rowline, keyname, raw_value;
		std::getline(in, rowline);
		bool success = _parseRule(rowline, section, keyname, raw_value);
		if (!success) {
			_log += "Warning: Line:" + to_string(lineCnt) + " Parse failed: '" +
					rowline + "'.\n";
			_is_sucess = false;
			continue;
		}
		if (keyname == "") {
			continue;
		}
		Field f(section, keyname);
		auto e = _elements.find(f);
		if (e == _elements.end()) {
			_log += "Warning: Line:" + to_string(lineCnt) + " Unknown key: ";
			if (section != "")
				_log += section + ".";
			_log += keyname + ".\n";
			_is_sucess = false;
			continue;
		}

		if (e->second.is_exist) {
			_log += "Warning: Line:" + to_string(lineCnt) +
					" Element is configured multiple times: ";
			if (section != "")
				_log += section + ".";
			_log += keyname + ".\n";
			_is_sucess = false;
			continue;
		}
		e->second.is_exist = true;

		auto cf = e->second.check_func;
		switch (e->second.type) {
		case Type::INT:
			int val;
			try {
				val = std::stoi(raw_value);
			} catch (...) {
				_is_sucess = false;
				_log += "Warning: Line:" + to_string(lineCnt) + " key: ";
				if (section != "")
					_log += section + ".";
				_log += keyname + " need int but read '" + raw_value + "'.\n";
				continue;
			}
			if (cf.Int != nullptr && cf.Int(val) == false) {
				_is_sucess = false;
				_log +=
					"Warning: Line:" + to_string(lineCnt) + " Invalid Value: ";
				if (section != "")
					_log += section + ".";
				_log += keyname + " = " + to_string(val) + ".\n";
				continue;
			}
			e->second.real_value.Int = val;
			break;
		case Type::STR:
			if (cf.Str != nullptr && cf.Str(raw_value) == false) {
				_is_sucess = false;
				_log +=
					"Warning: Line:" + to_string(lineCnt) + " Invalid Value: ";
				if (section != "")
					_log += section + ".";
				_log += keyname + "='" + raw_value + "'.\n";
				continue;
			}
			e->second.real_value.Str = raw_value;
		default:
			break;
		}
	}

	for (auto e = _elements.begin(); e != _elements.end(); e++) {
		Value &v = e->second.real_value;
		Type t = e->second.type;
		Field f = e->first;
		e->second.real_value = v;
		switch (t) {
		case Type::INT:
			if (_bind_int.count(f) != 0) {
				*(_bind_int[f]) = v.Int;
			}
			break;
		case Type::STR:
			if (_bind_string.count(f) != 0) {
				*(_bind_string[f]) = v.Str;
			}
			break;

		default:
			break;
		}
	}
}
} // namespace ini
} // namespace EUkit