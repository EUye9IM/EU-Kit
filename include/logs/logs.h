#ifndef EUKIT_LOGS_LOGS
#define EUKIT_LOGS_LOGS
#include <chrono>
#include <cstring>
#include <fstream>
#include <functional>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <string>

namespace EUkit {
namespace logs {
class Logs;
enum Level { LTRACE, LDEBUG, LINFO, LWARN, LERROR };
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
const static int VT100 = 1;
const static int DATE = 2;
const static int TIME = 4;
const static int MICRO_SEC = 8;
const static int GMT_TIME = 16;
const static int DEFAULT_THEME = DATE | TIME;

template <int flag = DEFAULT_THEME>
std::string def_theme(const std::string &msg, Level level) {
	static const char *CLEAR = "\33[0m";
	static const char *BLUE = "\33[94m";
	static const char *CYAN = "\33[96m";
	static const char *GREEN = "\33[92m";
	static const char *YELLOW = "\33[93m";
	static const char *RED = "\33[91m";
	static const char *FLAG[] = {"[ Trace ] ", "[ Debug ] ", "[ Info  ] ",
								 "[ Warn  ] ", "[ Error ] "};
	static const char *COLOR[] = {BLUE, CYAN, GREEN, YELLOW, RED};
	if (level < 0 || level >= 5)
		return "";

	auto t = std::chrono::system_clock::now();
	auto tt = std::chrono::system_clock::to_time_t(t);
	tm ttm;
	if (flag & GMT_TIME) {
		memcpy(&ttm, gmtime(&tt), sizeof(ttm));
	} else {
		memcpy(&ttm, localtime(&tt), sizeof(ttm));
	}
	int micro_sec = std::chrono::duration_cast<std::chrono::microseconds>(
						t.time_since_epoch())
						.count() %
					1000000;

	std::stringstream log_buf;
	int prefix_len = 0;

	// prefix
	if (flag & VT100) {
		log_buf << COLOR[level];
	}
	if (flag & DATE) {
		log_buf << std::setfill('0') << std::setw(4) << ttm.tm_year + 1900
				<< "-" << std::setw(2) << ttm.tm_mon + 1 << "-" << std::setw(2)
				<< ttm.tm_mday << " ";
		prefix_len += 11;
	}
	if (flag & TIME) {
		log_buf << std::setfill('0') << std::setw(2) << ttm.tm_hour << ":"
				<< std::setw(2) << ttm.tm_min << ":" << std::setw(2)
				<< ttm.tm_sec;
		prefix_len += 8;
		if (flag & MICRO_SEC) {
			log_buf << "." << std::setfill('0') << std::setw(6) << micro_sec;
			prefix_len += 7;
		}
		log_buf << " ";
		prefix_len += 1;
	}
	log_buf << FLAG[level];
	prefix_len += strlen(FLAG[level]);

	// handle message
	std::string newline_prefix("\n"), new_msg(msg);
	newline_prefix.append(prefix_len - 2, ' ');
	newline_prefix += "> ";
	for (std::string::size_type pos(0); pos != std::string::npos;
		 pos += newline_prefix.length()) {
		if ((pos = new_msg.find("\n", pos)) != std::string::npos) {
			new_msg.replace(pos, strlen("\n"), newline_prefix);
		} else
			break;
	}

	log_buf << new_msg;
	if (flag & VT100) {
		log_buf << CLEAR;
	}

	return log_buf.str();
}

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
