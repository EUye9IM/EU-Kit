#ifndef EUKIT_ARGS_TEXT
#define EUKIT_ARGS_TEXT
namespace EUkit {
namespace args {
namespace _internal {
static const char *UNKNOWN_TYPE = "Unexpect type.";

static const char *ARG_BAD_NAME = "Bad argument name.";
static const char *ARG_REPET = "Repetitive argument name.";

static const char *PARSER_BAD_SHORTNAME = "Bad short name.";
static const char *PARSER_BAD_LONGNAME = "Bad long name.";

static const char *TYPE_NAME[] = {"", "{integer}", "{decimal}", "{string}"};

static const char *INFO_USSAGE = "Ussage :";
static const char *INFO_OPTIONS = "Options :";
static const char *INFO_UNREQ_OPTIONS = "[options]";

static const char *PARSE_MUTI = "Parse multiple times.";
static const char *PARSE_ERR = "Error : ";
static const char *PARSE_SUCCESS = "Success.";
static const char *PARSE_RESTPARAM =
	" Extra or unknown parameters are prohibited.";
static const char *PARSE_MULTIPARAM = " Parameters are set multiple times.";

static const char *PARSE_MISSING = " Missing argument.";
static const char *PARSE_NOTENOUGH = " Not enough value.";
static const char *PARSE_TYPEERR = " Wrong value type.";
static const char *PARSE_CHECKFAILED = " Illegal value.";

} // namespace _internal
} // namespace args
} // namespace EUkit
#endif