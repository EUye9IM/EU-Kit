#include <args/main.h>

namespace EUkit {
namespace args {
namespace _internal {
// MULTITYPES
const int g_type_param_num[] = {0, 1, 1, 1};

const char *UNKNOWN_TYPE = "Unexpect type.";

const char *ARG_BAD_NAME = "Bad argument name.";
const char *ARG_REPET = "Repetitive argument name.";

const char *PARSER_BAD_SHORTNAME = "Bad short name.";
const char *PARSER_BAD_LONGNAME = "Bad long name.";

const char *TYPE_NAME[] = {"", "{integer}", "{decimal}", "{string}"};

const char *INFO_USSAGE = "Ussage :";
const char *INFO_OPTIONS = "Options :";
const char *INFO_UNREQ_OPTIONS = "[options]";

const char *PARSE_MUTI = "Parse multiple times.";
const char *PARSE_ERR = "Error : ";
const char *PARSE_SUCCESS = "Success.";
const char *PARSE_RESTPARAM =
	" Extra or unknown parameters are prohibited.";
const char *PARSE_MULTIPARAM = " Parameters are set multiple times.";

const char *PARSE_MISSING = " Missing argument.";
const char *PARSE_NOTENOUGH = " Not enough value.";
const char *PARSE_TYPEERR = " Wrong value type.";
const char *PARSE_CHECKFAILED = " Illegal value.";
} // namespace _internal
} // namespace args
} // namespace EUkit
