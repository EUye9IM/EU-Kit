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

	cout << "set prefix" << endl;
	log.setPrefix("prefix: ");
	log[LINFO] << "info";
	cout << "clear prefix" << endl;
	log.setPrefix("");
	log[LINFO] << "info";

	Logs sublog(log,"submodule: ");
	sublog[LINFO] << "info";

	cout << "colorful theme" << endl;
	log.setTheme(vt100_theme);
	log[LDEBUG] << "debug: blue";
	log[LINFO] << "info: green";
	sublog[LINFO] << "sub log also turn to green";
	log[LWARN] << "warning: yellow";
	log[LERR] << "error: red";

	cout<<"you can also create sublog by a sublog"<<endl;
	Logs subsublog(sublog,"anothersub: ");
	subsublog[LINFO] << "info";


	return 0;
}