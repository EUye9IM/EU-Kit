#include <ini/ini.h>

#include <iostream>
#include <regex>

using namespace std;
using namespace EUkit::ini;

bool conf2ParserRule(const std::string &textline, std::string &section,
					 std::string &keyname, std::string &value) {
	string textline2 = textline;
	auto pos = textline.find("//");
	if (pos != string::npos) {
		textline2 = textline.substr(0, pos);
	}
	string sectionPattern = "(?:\\[(?:\\s*)([^\\]\\s]+)(?:\\s*)\\])";
	string kvPattern = "(?:([^\\s=]+)(?:\\s*)=(?:\\s*)([^\\s]+))";
	string pattern =
		"^(?:\\s*)(?:" + sectionPattern + "|" + kvPattern + ")?(?:\\s*)$";

	regex re(pattern);
	smatch m;
	bool ret;
	ret = regex_match(textline2, m, re);
	string s = m[1].str();
	keyname = m[2].str();
	value = m[3].str();
	if (s != "") {
		section = s;
	}
	return ret;
}

bool checkPort(int p) { return 0 <= p && p <= 65535; }

void conf1() {
	cout << "========= conf1 =========" << endl;
	string ip = "0.0.0.0";
	int port = 8888;

	Parser p;
	p.bindString(
		 "", "服务器IP地址", ip,
		 [](const string &s) {
			 int a, b, c, d;
			 char e;
			 if (4 == sscanf(s.c_str(), "%d.%d.%d.%d%c", &a, &b, &c, &d, &e)) {
				 if (0 <= a && a <= 255 && 0 <= b && b <= 255 && 0 <= c &&
					 c <= 255 && 0 <= d && d <= 255)
					 return true;
			 }
			 return false;
		 })
		.bindInt("", "端口号", port, checkPort)
		.addString("", "不绑定变量")
		.addInt("", "数据类型错误报错");

	p.parseFile("conf1.ini");
	if (!p.success()) {
		cout << p.errmsg() << endl;
	}
	cout << ip << ":" << port << endl;
	cout << "不绑定变量" << p.getString("", "不绑定变量") << endl;
}

bool conf2dbg4b(const string &str) {
	if (str.length() != 4) {
		return false;
	}
	for (int i = 0; i < 4; i++) {
		if (str[i] != '0' && str[i] != '1') {
			return false;
		}
	}
	return true;
}

void conf2() {
	cout << "========= conf2 =========" << endl;
	int con_port = 44567;
	int con_dev_con = 15;
	int con_dev_samp = 900;

	string db_dbname = "db1234567";
	string db_uname = "dbuser_1234567";
	string db_addr = "localhost";
	int db_port = 3306;
	string db_pass = "db.1234567";

	int sys_noans_timeout = 30;
	int sys_trans_timeout = 30;

	int log_sub = 1024;
	int log_main = 10240;

	int dbg_scr = 10;
	string dbg_t_p = "0000";
	string dbg_t_s = "0000";
	string dbg_d_p = "0000";
	string dbg_d_s = "0000";

	Parser p;
	p.setParseRule(conf2ParserRule)
		.bindInt("连接设置", "监听端口号", con_port, checkPort)
		.bindInt("连接设置", "设备连接间隔", con_dev_con,
				 [](int i) { return i >= 5 && i <= 600; })
		.bindInt("连接设置", "设备采样间隔", con_dev_samp,
				 [](int i) { return i > 0; })
		.bindString("数据库", "数据库名", db_dbname)
		.bindString("数据库", "用户名", db_uname)
		.bindString("数据库", "服务器IP地址", db_addr)
		.bindInt("数据库", "服务器端口号", db_port, checkPort)
		.bindString("数据库", "用户口令", db_pass)
		.bindInt("系统设置", "未应答超时", sys_noans_timeout,
				 [](int i) { return i >= 2 && i <= 120; })
		.bindInt("系统设置", "传输超时", sys_trans_timeout,
				 [](int i) { return i >= 2 && i <= 120; })
		.bindInt("系统设置", "分日志大小", log_sub, [](int i) { return i > 0; })
		.bindInt("系统设置", "主日志大小", log_main,
				 [](int i) { return i > 0; })
		.bindInt("DEBUG设置", "屏幕显示", log_main)
		.bindString("DEBUG设置", "tmp_packet", dbg_t_p, conf2dbg4b)
		.bindString("DEBUG设置", "tmp_socket", dbg_t_s, conf2dbg4b)
		.bindString("DEBUG设置", "dev_packet", dbg_d_p, conf2dbg4b)
		.bindString("DEBUG设置", "dev_socket", dbg_d_s, conf2dbg4b);
	p.parseFile("conf2.ini");
	if (!p.success()) {
		cout << p.errmsg() << endl;
	}

	cout << "[连接设置]" << endl;
	cout << "监听端口号   = " << con_port << endl;
	cout << "设备连接间隔 = " << con_dev_con << endl;
	cout << "设备采样间隔 = " << con_dev_samp << endl;
	cout << "[数据库]" << endl;
	cout << "数据库名     = " << db_dbname << endl;
	cout << "用户名       = " << db_uname << endl;
	cout << "服务器IP地址 = " << db_addr << endl;
	cout << "服务器端口号 = " << db_port << endl;
	cout << "用户口令     = " << db_pass << endl;
	cout << "[系统设置]" << endl;
	cout << "未应答超时   = " << sys_noans_timeout << endl;
	cout << "传输超时     = " << sys_trans_timeout << endl;
	cout << "分日志大小   = " << log_sub << endl;
	cout << "主日志大小   = " << log_main << endl;
	cout << "[DEBUG设置]" << endl;
	cout << "屏幕显示     = " << dbg_scr << endl;
	cout << "tmp_packet   = " << dbg_t_p << endl;
	cout << "tmp_socket   = " << dbg_t_s << endl;
	cout << "dev_packet   = " << dbg_d_p << endl;
	cout << "dev_socket   = " << dbg_d_s << endl;
}

int main() {
	conf1();
	conf2();
	return 0;
}
/*

*/