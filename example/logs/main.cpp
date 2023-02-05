#include <logs/logs.h>

#include <iostream>
using namespace std;
using namespace EUkit::logs;

int main() {
	cout << "to file" << endl;
	Logs log("text.log");
	log[LINFO] << "balabalabala...";

	cout << "to screen" << endl;
	log.open(cout);
	log[LINFO] << "balabalabala...";

	cout << "mutiline" << endl;
	log[LINFO] << "bala...\nbalabala...\nbalabalabala...";

	cout << "level(default: LINFO)" << endl;
	log[LDEBUG] << "debug(disable)";
	log[LINFO] << "info";
	log[LWARN] << "warning";
	log[LERR] << "error";
	cout << "change level to LDEBUG" << endl;
	log.setLevel(LDEBUG);
	log[LDEBUG] << "debug(enable)";
	log[LINFO] << "info";
	log[LWARN] << "warning";
	log[LERR] << "error";
	
	cout << "colorful theme" << endl;
	log.setTheme(colorful_theme);
	log[LDEBUG] << "debug: blue";
	log[LINFO] << "info: green";
	log[LWARN] << "warning: yellow";
	log[LERR] << "error: red";
	return 0;
}