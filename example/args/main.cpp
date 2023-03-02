#include <args/args.h>
#include <iostream>

using namespace std;
using namespace EUkit::args;

int main(int argc, char **argv) {
	ArgParser p(argv[0]);
	p.enableRestParam()
		.add(newArg('h', "help")
				 ->info("show ussage information")) // 默认类型Flag
		.add(newArg("port")
				 ->type(Int)
				 ->defaultValue<Int>(8080)
				 ->check<Int>([](const int p) { return p >= 0 && p <= 65535; })
				 ->info("serve's port (Default : 8080)"))
		.add(newArg('s')
				 ->type(Dec)
				 ->info("transport speed(MBps) (Default : 3)")
				 ->defaultValue<Dec>(3.0))
		.add(newArg('p', "ip")
				 ->type(Str)
				 ->info("ip address")
				 ->defaultValue<Str>("0.0.0.0"));
	// 如果不指定默认值，则为必选选项（Flag类型除外）

	if (p.parse(argc, argv)) {
		cout << p.errorMsg() << endl << endl;
		cout << p.helpInfo() << endl;
	}

	if (p.val<Flag>("help")) {
		cout << p.helpInfo() << endl;
		cout << endl;
	}

	cout << "port  : " << p.val<Int>("port") << endl;
	cout << "speed : " << p.val<Dec>('s') << endl;
	cout << "ip    : " << p.val<Str>("ip") << endl;

	return 0;
}