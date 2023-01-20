#include <args/args.h>
#include <args/text.h>
using namespace std;

namespace EUkit {
namespace args {
namespace _internal {
Argument::Argument(char short_name, const std::string &long_name)
	: _short_name(short_name), _long_name(long_name), _is_required(true),
	  _is_exist(false), _info(""), _type(Type::Flag) {
	if (short_name == 0 && long_name.length() == 0) {
		throw std::logic_error(ARG_BAD_NAME);
	}
}
ArgumentPointer Argument::type(Type arg_type) {
	_type = arg_type;
	return shared_from_this();
}
ArgumentPointer Argument::info(const std::string &information) {
	_info = information;
	return shared_from_this();
}
std::string Argument::getLongName() {
	if (_long_name.length() != 0) {
		return "--" + _long_name;
	}
	return string("-") + _short_name;
}
bool Argument::isRequired() {
	return _is_required && (g_type_param_num[_type] != 0);
}
// MULTITYPES
template <>
ArgumentPointer Argument::check<Flag>(
	std::function<bool(const typename RealType<Flag>::Type)> const
		check_function) {
	_checkfunc.Flag = check_function;
	return shared_from_this();
}
template <>
ArgumentPointer Argument::check<Int>(
	std::function<bool(const typename RealType<Int>::Type)> const
		check_function) {
	_checkfunc.Int = check_function;
	return shared_from_this();
}
template <>
ArgumentPointer Argument::check<Dec>(
	std::function<bool(const typename RealType<Dec>::Type)> const
		check_function) {
	_checkfunc.Dec = check_function;
	return shared_from_this();
}
template <>
ArgumentPointer Argument::check<Str>(
	std::function<bool(const typename RealType<Str>::Type)> const
		check_function) {
	_checkfunc.Str = check_function;
	return shared_from_this();
}
// MULTITYPES
template <>
ArgumentPointer
Argument::defaultValue<Int>(const typename RealType<Int>::Type default_value) {
	_is_required = false;
	_value.Int = default_value;
	return shared_from_this();
}
template <>
ArgumentPointer
Argument::defaultValue<Dec>(const typename RealType<Dec>::Type default_value) {
	_is_required = false;
	_value.Dec = default_value;
	return shared_from_this();
}
template <>
ArgumentPointer
Argument::defaultValue<Str>(const typename RealType<Str>::Type default_value) {
	_is_required = false;
	_value.Str = default_value;
	return shared_from_this();
}

bool Argument::match(const std::string &str) {
	if (_short_name) {
		if (str == string("-") + _short_name)
			return true;
	}
	if (_long_name.length() != 0) {
		if (str == "--" + _long_name)
			return true;
	}
	return false;
}
int Argument::setArg(int argc, int &pos, char *const *const argv) {
	if (pos + g_type_param_num[_type] >= argc)
		return RET_NOTENOUGH;
	// MULTITYPES
	switch (_type) {
	case Flag:
		_value.Flag = true;
		break;
	case Int: {
		int ret, val;
		char c;
		ret = sscanf(argv[pos+1], "%d%c", &val, &c);
		if (1 == ret) {
			_value.Int = val;
		} else
			return RET_TYPEERR;
	} break;
	case Dec: {
		double val;
		int ret;
		char c;
		ret = sscanf(argv[pos+1], "%lf%c", &val, &c);
		if (1 == ret) {
			_value.Dec = val;
		} else
			return RET_TYPEERR;
	} break;
	case Str:
		_value.Str = argv[pos+1];
		break;
	default:
		throw logic_error(UNKNOWN_TYPE);
		break;
	}

	pos += g_type_param_num[_type];

	// MULTITYPES
	switch (_type) {
	case Flag:
		if (_checkfunc.Flag && (!_checkfunc.Flag(_value.Flag)))
			return RET_CHECKFAILED;
		break;
	case Int:
		if (_checkfunc.Int && (!_checkfunc.Int(_value.Int)))
			return RET_CHECKFAILED;
		break;
	case Dec:
		if (_checkfunc.Dec && (!_checkfunc.Dec(_value.Dec)))
			return RET_CHECKFAILED;
		break;
	case Str:
		if (_checkfunc.Str && (!_checkfunc.Str(_value.Str)))
			return RET_CHECKFAILED;
		break;
	default:
		throw logic_error(UNKNOWN_TYPE);
		break;
	}
	pos++;
	return RET_OK;
}
// MULTITYPES

} // namespace _internal
_internal::ArgumentPointer newArg(char short_name) {
	return std::make_shared<_internal::Argument>(short_name, "");
}
_internal::ArgumentPointer newArg(const std::string &long_name) {
	return std::make_shared<_internal::Argument>(NULL, long_name);
}
_internal::ArgumentPointer newArg(char short_name,
								  const std::string &long_name) {
	return make_shared<_internal::Argument>(short_name, long_name);
}
} // namespace args
} // namespace EUkit