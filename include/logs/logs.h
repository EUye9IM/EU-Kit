#ifndef EUKIT_LOGC_LOGS
#define EUKIT_LOGC_LOGS
#include <fstream>
#include <functional>
#include <sstream>
#include <string>

namespace EUkit {
namespace logs {
class Logs;
enum Level { LDEBUG, LINFO, LWARN, LERR};
namespace _internal {
class _Buf {
public:
	_Buf(Logs *plogs, Level level);
	_Buf(_Buf&& b);
	~_Buf();
	template <typename T> _Buf &operator<<(const T value) {
		buf << value;
		return *this;
	}

private:
	Logs *plogs;
	Level level;
	std::stringstream buf;
	bool is_moved;
};


} // namespace _internal
using LogTheme = std::function<std::string(const std::string &,Level)>;
extern LogTheme default_theme;
extern LogTheme colorful_theme;

class Logs {
	friend class _internal::_Buf;

public:
	Logs();
	Logs(std::ostream &out);
	Logs(const std::string &filename);
	~Logs();
	void setTheme(const LogTheme);
	void setLevel(Level level);
	void open(std::ostream &out);
	int open(const std::string &filename);
	_internal::_Buf operator[](Level level);
	// _internal::_Buf debug;
	// _internal::_Buf info;
	// _internal::_Buf warning;
	// _internal::_Buf error;

private:
	void close();
	void write(Level level, const std::string &msg);

	std::ostream *out;
	std::ofstream outf;
	LogTheme theme;
	Level level;
};
} // namespace logs
} // namespace EUkit

#endif