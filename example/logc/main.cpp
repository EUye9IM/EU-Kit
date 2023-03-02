#include <logc/logc.h>

using EUkit::logc;

int main() {
	logc.printf("you can just print to screen\n");
	logc.debugf("and debug message will not be print\n");
	logc.addAttr(logc.A_DEBUG);
	logc.debugf("unless add the attribute 'A_DEBUG'\n");
	logc.debugf("and this is default attrbute: %d\n",logc.DEFAULT_ATTR);
	if (!logc.open("test.log")) {
		logc.printf("you can also print to file with open()\n");
	}
	logc.use(stdout);
	logc.printf("or using use() to print to a file pointer\n");
	logc.printf("when using open(), LogC will close File automaticly.\n");
	logc.printf("when using use(), will not.\n");
	logc.fatalf("you can use fatalf() to print message and exit\n");
	logc.printf("so you will not see this message.\n");

	return 0;
}