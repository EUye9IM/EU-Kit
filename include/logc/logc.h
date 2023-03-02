#ifndef EUKIT_LOGC_LOGC
#define EUKIT_LOGC_LOGC
#include <cstdio>

namespace EUkit {
class LogC {
public:
	// attribute
	static const unsigned int A_TIME = 1 << 0;
	static const unsigned int A_UTC = 1 << 1;
	static const unsigned int A_DATE = 1 << 2;
	static const unsigned int A_DEBUG = 1 << 3;

	static const unsigned int DEFAULT_ATTR = A_DATE | A_TIME;

	LogC();
	LogC(const char *file_path);
	LogC(FILE *file);
	~LogC();

	int open(const char *file_path);
	int use(FILE *file);

	int close();

	unsigned int addAttr(unsigned int attribute);
	unsigned int removeAttr(unsigned int attribute);
	unsigned int setAttr(unsigned int attribute);
	unsigned int getAttr();

	int printf(const char *fmt, ...);
	int debugf(const char *fmt, ...);
	int fatalf(const char *fmt, ...);

private:
	inline void _printHead();
	FILE *_fp;
	bool _need_close_file;
	unsigned int _attr;
};

extern LogC logc;

} // namespace EUkit
#endif