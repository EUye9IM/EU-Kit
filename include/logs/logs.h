#ifndef EUKIT_LOGS_LOGS
#define EUKIT_LOGS_LOGS
#include <fstream>
#include <functional>
#include <mutex>
#include <sstream>
#include <string>

namespace EUkit {
namespace logs {
class Logs;
enum Level { LDEBUG, LINFO, LWARN, LERR };
namespace _internal {
class _Buf {
public:
	_Buf(Logs *plogs, Level level);
	_Buf(_Buf &&b);
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
using LogTheme = std::function<std::string(const std::string &, Level)>;
extern LogTheme default_theme;
extern LogTheme vt100_theme;

class Logs {
	friend class _internal::_Buf;

public:
	Logs();
	Logs(std::ostream &out);
	Logs(const std::string &filename);
	Logs(Logs &log, const std::string &prefix = std::string());
	~Logs();
	Logs &setTheme(const LogTheme);
	Logs &setLevel(Level level);
	Logs &setPrefix(const std::string &prefix);
	Logs &open(std::ostream &out);
	Logs &open(const std::string &filename);
	Logs &open(Logs &log);
	_internal::_Buf operator[](Level level);

private:
	void close();
	void write(Level level, const std::string &msg);
	std::string prefix;
	Logs *log;
	std::ostream *out;
	std::ofstream outf;
	LogTheme theme;
	Level level;
	std::mutex write_lock;
};
} // namespace logs
} // namespace EUkit

#endif
