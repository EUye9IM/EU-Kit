#include <logc/logc.h>

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <ctime>

namespace EUkit {
// gloable
LogC logc(stdout);
static const int ERR = -1;
static const int OK = 0;

// class function
LogC::LogC() {
	_fp = nullptr;
	_need_close_file = false;
	_attr = DEFAULT_ATTR;
}
LogC::LogC(const char *file_path) {
	_need_close_file = false;
	open(file_path);
	_attr = DEFAULT_ATTR;
}
LogC::LogC(FILE *file) {
	_need_close_file = false;
	use(file);
	_attr = DEFAULT_ATTR;
}

LogC::~LogC() { close(); }

int LogC::close() {
	if (_need_close_file && _fp)
		return fclose(_fp);
	_need_close_file = false;
	return OK;
}

int LogC::open(const char *file_path) {
	close();
	_fp = fopen(file_path, "a");
	if (!_fp)
		return ERR;
	_need_close_file = true;
	return OK;
}
int LogC::use(FILE *file) {
	close();
	_fp = file;
	_need_close_file = false;
	return OK;
}

unsigned int LogC::addAttr(unsigned int attribute) {
	_attr |= attribute;
	return _attr;
}
unsigned int LogC::removeAttr(unsigned int attribute) {
	_attr &= ~attribute;
	return _attr;
}
unsigned int LogC::setAttr(unsigned int attribute) {
	_attr = attribute;
	return _attr;
}
unsigned int LogC::getAttr() { return _attr; }

int LogC::printf(const char *fmt, ...) {
	if (!_fp)
		return ERR;
	_printHead();
	va_list ap;
	va_start(ap, fmt);
	int ret = vfprintf(_fp, fmt, ap);
	va_end(ap);
	fflush(_fp);
	return ret;
}
int LogC::debugf(const char *fmt, ...) {
	if (!_fp)
		return ERR;
	if (!(_attr & A_DEBUG))
		return OK;
	_printHead();
	va_list ap;
	va_start(ap, fmt);
	int ret = vfprintf(_fp, fmt, ap);
	va_end(ap);
	fflush(_fp);
	return ret;
}
int LogC::fatalf(const char *fmt, ...) {
	if (!_fp)
		return ERR;
	_printHead();
	va_list ap;
	va_start(ap, fmt);
	int ret = vfprintf(_fp, fmt, ap);
	va_end(ap);
	fflush(_fp);
	exit(0);
	return ret;
}

inline void LogC::_printHead() {
	if (!_fp)
		return;
	static time_t cur;
	static tm *c_tm;

	if (_attr & (A_DATE | A_TIME)) {
		cur = time(NULL);
		if (_attr & A_UTC)
			c_tm = gmtime(&cur);
		else
			c_tm = localtime(&cur);
	}

	if (_attr & A_DATE) {
		fprintf(_fp, "%04d/%02d/%02d ", c_tm->tm_year + 1900, c_tm->tm_mon + 1,
				c_tm->tm_mday);
	}
	if (_attr & A_TIME) {
		fprintf(_fp, "%02d:%02d:%02d ", c_tm->tm_hour, c_tm->tm_min,
				c_tm->tm_sec);
	}
}
} // namespace EUkit