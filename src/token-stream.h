/*
   Copyright (c) 2015, Andreas Fett. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.
*/
#include <inttypes.h>
#include <string>

namespace Json {

class Token {
public:
	enum Type {
		END             = -1,
		INVALID         = '\0',
		BEGIN_ARRAY     = '[',
		BEGIN_OBJECT    = '{',
		END_ARRAY       = ']',
		END_OBJECT      = '}',
		NAME_SEPARATOR  = ':',
		VALUE_SEPARATOR = ',',
		TRUE_LITERAL    = 't',
		FALSE_LITERAL   = 'f',
		NULL_LITERAL    = 'n',
		STRING          = '"',
		NUMBER          = '0',
	} type;

	enum NumberType {
		NONE,
		INT,
		FLOAT,
	} number_type;

	int64_t int_value;
	long double float_value;
	std::string str_value;

	Token()
	:
		type(INVALID),
		number_type(NONE),
		int_value(0),
		float_value(0.0L),
		str_value()
	{ }

	void reset()
	{
		type = INVALID;
		number_type = NONE;
		int_value = 0;
		float_value = 0.0L;
		str_value.clear();
	}
};

class Utf8Stream;

class TokenStream {
public:
	TokenStream(Utf8Stream &);

	void scan(); // throws jsonp::Error

	Token token;
private:
	typedef void (TokenStream::*scanner)(void);

	scanner select_scanner(int);

	void scan_structural();
	void scan_true();
	void scan_false();
	void scan_null();
	void scan_literal(const char *);
	void scan_string();
	void scan_number();

	Utf8Stream & stream_;
};

}
