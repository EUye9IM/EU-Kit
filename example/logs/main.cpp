#include <logs/logs.h>

#include <iostream>
using namespace std;
using namespace EUkit::logs;

int main() {
	cout << "to file" << endl;
	Logs log("test.log");
	log[LINFO] << "balabalabala...";

	cout << "to screen" << endl;
	log.open(cout);
	log[LINFO] << "balabalabala...";

	cout << "mutiline" << endl;
	log[LINFO] << "bala...\nbalabala...\nbalabalabala...";

	cout << "level(default: LINFO)" << endl;
	log[LTRACE] << "trace(disable)";
	log[LDEBUG] << "debug(disable)";
	log[LINFO] << "info";
	log[LWARN] << "warning";
	log[LERROR] << "error";
	cout << "change level to LDEBUG" << endl;
	log[LTRACE] << "trace(disable)";
	log.setLevel(LDEBUG);
	log[LDEBUG] << "debug(enable)";
	log[LINFO] << "info";
	log[LWARN] << "warning";
	log[LERROR] << "error";

	cout << "set prefix" << endl;
	log.setPrefix("prefix: ");
	log[LINFO] << "info";
	cout << "clear prefix" << endl;
	log.setPrefix("");
	log[LINFO] << "info";

	Logs sublog(log, "submodule: ");
	sublog[LINFO] << "info";

	cout << "change level to LTRACE" << endl;
	log.setLevel(LTRACE);
	cout << "colorful theme" << endl;
	log.setTheme(def_theme<VT100 | DATE | TIME | MICRO_SEC /* | GMT_TIME */>);
	log[LTRACE] << "trace: blue";
	log[LDEBUG] << "debug: cyan";
	log[LINFO] << "info: green";
	sublog[LINFO] << "sub log also turn to green";
	log[LWARN] << "warning: yellow";
	log[LERROR] << "error: red";

	cout << "you can also create sublog by a sublog" << endl;
	Logs subsublog(sublog, "anothersub: ");
	subsublog[LINFO] << "info";

	return 0;
}