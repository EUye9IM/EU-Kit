#include <exception>
#include <iostream>
#include <logs/logs.h>

using namespace std;
using namespace EUkit::logs;

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
	: prefix(), log(nullptr), out(&cout), outf(), theme(def_theme<>),
	  level(Level::LINFO) {}
Logs::Logs(std::ostream &out)
	: prefix(), log(nullptr), out(&out), outf(), theme(def_theme<>),
	  level(Level::LINFO) {}
Logs::Logs(const std::string &filename)
	: prefix(), log(nullptr), out(&outf), outf(filename, ios::out | ios::app),
	  theme(def_theme<>), level(Level::LINFO) {
	if (!outf) {
		throw runtime_error("cannot open file");
	}
}
Logs::Logs(Logs &log, const std::string &prefix)
	: prefix(prefix), log(&log), out(nullptr), outf(), theme(def_theme<>),
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
