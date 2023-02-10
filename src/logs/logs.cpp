#include <chrono>
#include <exception>
#include <iomanip>
#include <iostream>
#include <logs/logs.h>
#include <mutex>

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

static const char *CLEAR = "\33[0m";
static const char *GREEN = "\33[36m";
static const char *BLUE = "\33[34m";
static const char *YELLOW = "\33[33m";
static const char *RED = "\33[31m";
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
static const char *FLAG[] = {"[ Debug ] ", "[ Info  ] ", "[ Warn  ] ",
							 "[ Error ] "};
static const char *COLOR[] = {BLUE, GREEN, YELLOW, RED};

LogTheme EUkit::logs::default_theme([](const string &msg, Level level) {
	stringstream s;

	string msg2(msg);
	replace_all(msg2, "\n", "\n                            > ");
	addTimedate(s);
	s << FLAG[level] << msg2;

	return s.str();
});
LogTheme EUkit::logs::colorful_theme([](const string &msg, Level level) {
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
	string msg = this->plogs->theme(this->buf.str(), this->level);
	msg += "\n";
	this->plogs->write(this->level, msg);
}

} // namespace EUkit::logs::_internal

Logs::Logs() : out(&cout), outf(), theme(default_theme), level(Level::LINFO) {}
Logs::Logs(std::ostream &out)
	: out(&out), outf(), theme(default_theme), level(Level::LINFO) {}
Logs::Logs(const std::string &filename)
	: out(&outf), outf(filename, ios::out | ios::app), theme(default_theme),
	  level(Level::LINFO) {
	if (!outf) {
		throw runtime_error("cannot open file");
	}
}
Logs::~Logs() { this->close(); }
void Logs::setTheme(const LogTheme config) { this->theme = config; }
void Logs::setLevel(Level level) { this->level = level; }
void Logs::open(std::ostream &out) {
	close();
	this->out = &out;
}
int Logs::open(const std::string &filename) {
	close();
	outf.open(filename, ios::out | ios::app);
	if (!outf) {
		return -1;
	}
	out = &outf;
	return 0;
}

_internal::_Buf Logs::operator[](Level level) {
	return move(_internal::_Buf(this, level));
}
void Logs::close() {
	out = nullptr;
	if (outf.is_open()) {
		outf.close();
	}
	outf.clear();
}

void Logs::write(Level level, const std::string &msg) {
	static mutex lock;
	if (this->level > level)
		return;
	if (out) {
		lock_guard<mutex> locker(lock);
		*out << msg;
		out->flush();
	}
}
