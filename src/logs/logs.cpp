#include <chrono>
#include <exception>
#include <iomanip>
#include <iostream>
#include <logs/logs.h>

using namespace std;
using namespace EUkit::logs;

static void addTimedate(stringstream &ss) {
	auto t = chrono::system_clock::to_time_t(chrono::system_clock::now());
	// put_time
	auto tm = localtime(&t);
	ss << setfill('0') << setw(4) << tm->tm_year + 1900 << "-" << setw(2)
	   << tm->tm_mon + 1 << "-" << setw(2) << tm->tm_mday << " " << setw(2)
	   << tm->tm_hour << ":" << setw(2) << tm->tm_min << ":" << setw(2)
	   << tm->tm_sec << " ";
}

// static const char *CLEAR = "\33[0m";
// static const char *BLUE = "\33[34m";
// static const char *CYAN = "\33[36m";
// static const char *GREEN = "\33[32m";
// static const char *YELLOW = "\33[33m";
// static const char *RED = "\33[31m";

static const char *CLEAR = "\33[0m";
static const char *BLUE = "\33[94m";
static const char *CYAN = "\33[96m";
static const char *GREEN = "\33[92m";
static const char *YELLOW = "\33[93m";
static const char *RED = "\33[91m";

static string &replace_all(string &src, const string &old_value,
						   const string &new_value) {
	// 每次重新定位起始位置，防止上轮替换后的字符串形成新的old_value
	for (string::size_type pos(0); pos != string::npos;
		 pos += new_value.length()) {
		if ((pos = src.find(old_value, pos)) != string::npos) {
			src.replace(pos, old_value.length(), new_value);
		} else
			break;
	}
	return src;
}
static const char *FLAG[] = {"[ Trace ] ", "[ Debug ] ", "[ Info  ] ",
							 "[ Warn  ] ", "[ Error ] "};
static const char *COLOR[] = {BLUE, CYAN, GREEN, YELLOW, RED};

LogTheme EUkit::logs::default_theme([](const string &msg, Level level) {
	stringstream s;

	string msg2(msg);
	replace_all(msg2, "\n", "\n                            > ");
	addTimedate(s);
	s << FLAG[level] << msg2;

	return s.str();
});
LogTheme EUkit::logs::vt100_theme([](const string &msg, Level level) {
	stringstream s;
	s << COLOR[level];
	string msg2(msg);
	replace_all(msg2, "\n", "\n                            > ");
	addTimedate(s);
	s << FLAG[level] << msg2 << CLEAR;

	return s.str();
});

namespace EUkit::logs::_internal {
_Buf::_Buf(Logs *plogs, Level level)
	: plogs(plogs), level(level), is_moved(false) {}
_Buf::_Buf(_Buf &&b)
	: plogs(b.plogs), level(b.level), buf(move(b.buf)), is_moved(false) {
	b.is_moved = true;
}
_Buf::~_Buf() {
	if (is_moved)
		return;
	this->plogs->write(this->level, this->buf.str());
}

} // namespace EUkit::logs::_internal

Logs::Logs()
	: prefix(), log(nullptr), out(&cout), outf(), theme(default_theme),
	  level(Level::LINFO) {}
Logs::Logs(std::ostream &out)
	: prefix(), log(nullptr), out(&out), outf(), theme(default_theme),
	  level(Level::LINFO) {}
Logs::Logs(const std::string &filename)
	: prefix(), log(nullptr), out(&outf), outf(filename, ios::out | ios::app),
	  theme(default_theme), level(Level::LINFO) {
	if (!outf) {
		throw runtime_error("cannot open file");
	}
}
Logs::Logs(Logs &log, const std::string &prefix)
	: prefix(prefix), log(&log), out(nullptr), outf(), theme(default_theme),
	  level(Level::LINFO) {}
Logs::~Logs() { this->close(); }
Logs &Logs::setTheme(const LogTheme config) {
	this->theme = config;
	return *this;
}
Logs &Logs::setLevel(Level level) {
	this->level = level;
	return *this;
}
Logs &Logs::setPrefix(const string &prefix) {
	this->prefix = prefix;
	return *this;
}
Logs &Logs::open(std::ostream &out) {
	close();
	this->out = &out;
	return *this;
}
Logs &Logs::open(const std::string &filename) {
	close();
	outf.open(filename, ios::out | ios::app);
	if (!outf) {
		throw runtime_error("cannot open file");
	}
	out = &outf;
	return *this;
}
Logs &Logs::open(Logs &log) {
	close();
	this->log = &log;
	return *this;
}

_internal::_Buf Logs::operator[](Level level) {
	return move(_internal::_Buf(this, level));
}
void Logs::close() {
	log = nullptr;
	out = nullptr;
	if (outf.is_open()) {
		outf.close();
	}
	outf.clear();
}

void Logs::write(Level level, const std::string &msg) {
	if (this->level > level)
		return;
	lock_guard<mutex> locker(write_lock);
	if (log) {
		log[0][level] << prefix + msg;
	} else if (out) {
		*out << theme(prefix + msg, level) << "\n";
		out->flush();
	}
}
