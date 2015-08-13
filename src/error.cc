/*
   Copyright (c) 2015, Andreas Fett. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.
*/
#include <jsoncc.h>

namespace Json {

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
	"maximum nesting depth reached",
	"document contains bad token",
	"array starts with bad element",
	"expected ',' or ']' after array element",
	"array contains bad element",
	"object is not empty and does not start with a key",
	"expected ':' after object key",
	"bad or missing  value in object member",
	"expected ',' or '}' after object member",
	"object contains bad member",
	"internal error",
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
