#ifndef EUKIT_ARGS_ARGS
#define EUKIT_ARGS_ARGS
#include <args/text.h>
#include <args/types.h>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
namespace EUkit {
namespace args {
class ArgParser;
// private

namespace _internal {

enum _Ret { RET_OK, RET_NOTENOUGH, RET_TYPEERR, RET_CHECKFAILED };
// 允许智能指针链式调用
class Argument : public std::enable_shared_from_this<Argument> {
	using ArgumentPointer = std::shared_ptr<Argument>;
	friend class ::EUkit::args::ArgParser;

public:
	Argument(char short_name, const std::string &long_name);

	// 指定参数类型
	ArgumentPointer type(Type arg_type);

	// 指定参数校验
	template <Type T>
	ArgumentPointer
	check(std::function<bool(const typename RealType<T>::Type)> const
			  check_function) {
		throw std::out_of_range(UNKNOWN_TYPE);
		return shared_from_this();
	}

	// 设定参数提示信息
	ArgumentPointer info(const std::string &infomation);

	// 设定参数默认值
	template <Type T>
	ArgumentPointer
	defaultValue(const typename RealType<T>::Type default_value) {
		throw std::out_of_range(UNKNOWN_TYPE);
		return shared_from_this();
	}

private:
	char _short_name;
	std::string _long_name;
	bool _is_required;
	bool _is_exist;
	std::string _info;
	std::vector<std::string> _recv_param;
	Type _type;
	Value _value;
	CheckFunc _checkfunc;

	std::string getLongName();
	bool isRequired();
	bool match(const std::string &str);
	int setArg(int argc, int &pos, char *const *const argv);
};
using ArgumentPointer = std::shared_ptr<Argument>;
} // namespace _internal
class ArgParser {
public:
	ArgParser(const std::string &proj_name);
	ArgParser &add(_internal::ArgumentPointer argument);
	ArgParser &enableRestParam();
	template <Type T>
	typename _internal::RealType<T>::Type val(char short_name) {
		throw std::out_of_range(_internal::UNKNOWN_TYPE);
		typename _internal::RealType<T>::Type val;
		return val;
	};
	template <Type T>
	typename _internal::RealType<T>::Type val(const std::string &long_name) {
		throw std::out_of_range(_internal::UNKNOWN_TYPE);
		typename _internal::RealType<T>::Type val;
		return val;
	};
	std::vector<std::string> restParams();
	int parse(int argc, char *const *const argv);
	std::string helpInfo();
	std::string errorMsg();

private:
	std::string _proj_name;
	bool _enable_rest_param;
	std::vector<std::string> _rest_params;
	std::vector<_internal::ArgumentPointer> _args;
	bool _is_success;
	bool _has_parsed;
	std::string _err_msg;
	_internal::Value _val(char short_name);
	_internal::Value _val(const std::string &long_name);
};
// public

// create argument
_internal::ArgumentPointer newArg(char short_name);
_internal::ArgumentPointer newArg(const std::string &long_name);
_internal::ArgumentPointer newArg(char short_name,
								  const std::string &long_name);

} // namespace args
} // namespace EUkit
#endif