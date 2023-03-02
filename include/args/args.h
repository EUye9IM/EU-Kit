#ifndef EUKIT_ARGS_ARGS
#define EUKIT_ARGS_ARGS
#include <args/main.h>
namespace EUkit {
namespace args {
// MULTITYPES
template <> bool ArgParser::val<Flag>(char short_name) {
	return _val(short_name).Flag;
}
template <> bool ArgParser::val<Flag>(const std::string &long_name) {
	return _val(long_name).Flag;
}
template <> int ArgParser::val<Int>(char short_name) {
	return _val(short_name).Int;
}
template <> int ArgParser::val<Int>(const std::string &long_name) {
	return _val(long_name).Int;
}
template <> double ArgParser::val<Dec>(char short_name) {
	return _val(short_name).Dec;
}
template <> double ArgParser::val<Dec>(const std::string &long_name) {
	return _val(long_name).Dec;
}
template <> std::string ArgParser::val<Str>(char short_name) {
	return _val(short_name).Str;
}
template <> std::string ArgParser::val<Str>(const std::string &long_name) {
	return _val(long_name).Str;
}

namespace _internal {

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
} // namespace _internal
} // namespace args
} // namespace EUkit
#endif