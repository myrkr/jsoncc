#include <jsonp.h>

namespace jsonp {

static const char *error_message[] = {
	"no error",
	"ascii zero in source",
	"invalid utf8 in source",
	"unrecognized token",
	"bad literal",
	"unescaped control char in string",
	"unterminated string (at eof)",
	"invalid escape char",
	"invalide unicode escape",
	"unicode escape yields zero",
	"unicode escape yields surrogate",
	"invalid number format",
	"number too long",
};

Location::Location(size_t offs_, size_t character_, size_t line_)
:
	offs(offs_),
	character(character_),
	line(line_)
{ }

Error::Error(Type type_, Location location_)
:
	std::runtime_error(error_message[type_]),
	type(type_),
	location(location_)
{ }

}
