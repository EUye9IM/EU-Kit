#ifndef EUKIT_ARGS_TYPES
#define EUKIT_ARGS_TYPES

#include <string>
#include <functional>

namespace EUkit {
namespace args {
enum Type { Flag, Int, Dec, Str };
namespace _internal {


static const int g_type_param_num[] = {0, 1, 1, 1};


// Type to C++ type
template <Type T> struct RealType {};
template <> struct RealType<Type::Flag> {
	using Type = bool;
};
template <> struct RealType<Type::Int> {
	using Type = int;
};
template <> struct RealType<Type::Dec> {
	using Type = double;
};
template <> struct RealType<Type::Str> {
	using Type = std::string;
};

struct Value {
	RealType<Type::Flag>::Type Flag;
	RealType<Type::Int>::Type Int;
	RealType<Type::Dec>::Type Dec;
	RealType<Type::Str>::Type Str;
};
struct CheckFunc {
	std::function<bool(const typename RealType<Type::Flag>::Type)> Flag;
	std::function<bool(const typename RealType<Type::Int>::Type)> Int;
	std::function<bool(const typename RealType<Type::Dec>::Type)> Dec;
	std::function<bool(const typename RealType<Type::Str>::Type)> Str;
};

} // namespace _internal

} // namespace args
} // namespace EUkit
#endif
