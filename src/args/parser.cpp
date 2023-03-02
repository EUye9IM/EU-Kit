#include <args/main.h>

#include "util.h"

using namespace std;

namespace EUkit {
namespace args {

using namespace _internal;

// 设定应用名
ArgParser::ArgParser(const std::string &proj_name)
	: _proj_name(proj_name), _enable_rest_param(false), _is_success(true),
	  _has_parsed(false){};
// 添加参数
ArgParser &ArgParser::add(_internal::ArgumentPointer argument) {
	for (auto a = _args.begin(); a != _args.end(); a++) {
		if (static_cast<int>(argument->_short_name) != 0) {
			if (argument->_short_name == (*a)->_short_name)
				throw logic_error(ARG_REPET);
		}
		if (argument->_long_name != "") {
			if (argument->_long_name == (*a)->_long_name) {
				throw logic_error(ARG_REPET);
			}
		}
	}
	_args.push_back(argument);
	return *this;
}
// 允许多余参数
ArgParser &ArgParser::enableRestParam() {
	_enable_rest_param = true;
	return *this;
}

// 取参数值
_internal::Value ArgParser::_val(char short_name) {
	if (!short_name)
		throw logic_error(PARSER_BAD_SHORTNAME);
	for (auto a = _args.begin(); a != _args.end(); a++) {
		if (short_name == (*a)->_short_name) {
			return (*a)->_value;
		}
	}
	throw logic_error(PARSER_BAD_SHORTNAME);
}
// 取参数值
_internal::Value ArgParser::_val(const std::string &long_name) {
	if (long_name == "")
		throw logic_error(PARSER_BAD_LONGNAME);
	for (auto a = _args.begin(); a != _args.end(); a++) {
		if (long_name == (*a)->_long_name) {
			return (*a)->_value;
		}
	}
	throw logic_error(PARSER_BAD_LONGNAME);
}
// 取多余参数
std::vector<std::string> ArgParser::restParams() { return _rest_params; }
// 帮助信息
std::string ArgParser::helpInfo() {
	static const int pos1 = 4, pos2 = 2, pos3 = 22, blank = 2;
	string ret, tmp;

	ret += string(INFO_USSAGE) + " " + _proj_name;

	bool has_unrequire = false;

	for (auto a = _args.begin(); a != _args.end(); a++) {
		if ((*a)->isRequired()) {
			ret += string(" ") + (*a)->getLongName();
			if (*TYPE_NAME[(*a)->_type]) {
				ret += string(" ") + TYPE_NAME[(*a)->_type];
			}
		} else {
			has_unrequire = true;
		}
	}

	if (has_unrequire)
		ret += string(" ") + INFO_UNREQ_OPTIONS;
	if (_enable_rest_param)
		ret += " ...";

	ret += string("\n") + INFO_OPTIONS;
	for (auto a = _args.begin(); a != _args.end(); a++) {
		auto arg = *a;
		ret += "\n";
		ret.append(pos1, ' ');
		if (arg->_short_name)
			ret += string("-") + arg->_short_name;
		else
			ret += "  ";
		ret.append(pos2, ' ');
		tmp.clear();
		if (arg->_long_name.length() != 0) {
			tmp += "--" + arg->_long_name;
		}
		if (*TYPE_NAME[(*a)->_type]) {
			tmp += string(" ") + TYPE_NAME[(*a)->_type];
		}
		ret += tmp;
		if (arg->_info.length() != 0) {
			if (pos3 - static_cast<int>(tmp.length()) >= blank) {
				ret.append(pos3 - tmp.length(), ' ');
			} else {
				ret += "\n";
				ret.append(pos1 + 2 + pos2 + pos3, ' ');
			}
			ret += arg->_info;
		}
	}
	return ret;
}
// 错误信息
std::string ArgParser::errorMsg() { return _err_msg; }

// 分析参数
int ArgParser::parse(int argc, char *const *const argv) {
	// 第一次分析参数
	if (_has_parsed) {
		throw logic_error(PARSE_MUTI);
		return RET_BAD;
	}
	_has_parsed = true;

	int pos = 1;
	while (pos < argc) {
		string com = argv[pos];
		shared_ptr<Argument> arg;
		for (auto a = _args.begin(); a != _args.end(); a++) {
			if ((*a)->match(com)) {
				arg = (*a);
				break;
			}
		}
		// 不为参数或匹配不上，则认为是多余参数
		if (!arg) {
			if (!_enable_rest_param) {
				_err_msg = PARSE_ERR + com + PARSE_RESTPARAM;
				_is_success = false;
				return RET_BAD;
			}
			for (int i = pos; i < argc; i++) {
				_rest_params.push_back(argv[i]);
			}
			break;
		}
		// 匹配上，检查是否已经设定过
		if (arg->_is_exist) {
			_err_msg = PARSE_ERR + com + PARSE_MULTIPARAM;
			_is_success = false;
			return RET_BAD;
		}
		arg->_is_exist = true;
		// 检查是否有足够的参数以及是否能够转换是否合法
		int ret = arg->setArg(argc, pos, argv);
		switch (ret) {
		case RET_NOTENOUGH:
			_err_msg = PARSE_ERR + arg->getLongName() + PARSE_NOTENOUGH;
			_is_success = false;
			return RET_BAD;
			break;
		case RET_TYPEERR:
			_err_msg = PARSE_ERR + arg->getLongName() + PARSE_TYPEERR;
			_is_success = false;
			return RET_BAD;
			break;
		case RET_CHECKFAILED:
			_err_msg = PARSE_ERR + arg->getLongName() + PARSE_CHECKFAILED;
			_is_success = false;
			return RET_BAD;
			break;

		default:
			// ok, have no error
			break;
		}
	}
	// 检查所有参数，是否必要的已经指定了
	for (auto a = _args.begin(); a != _args.end(); a++) {
		if ((*a)->isRequired() && (!(*a)->_is_exist)) {
			_err_msg = PARSE_ERR + (*a)->getLongName() + PARSE_MISSING;
			_is_success = false;
			return RET_BAD;
		}
	}
	// 返回
	_err_msg = PARSE_SUCCESS;
	return 0;
}

} // namespace args

} // namespace EUkit