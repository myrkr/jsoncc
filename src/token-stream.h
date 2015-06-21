/*
   Copyright (c) 2015, Andreas Fett. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.
*/
#ifndef JSONP_TOKEN_STREAM_H
#define JSONP_TOKEN_STREAM_H

#include <iosfwd>

namespace jsonp {

class Token {
public:
	enum Type {
		INVALID         = '\0',
		BEGIN_ARRAY     = '[',
		BEGIN_OBJECT    = '}',
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

	Token() : type(INVALID) { }
};

std::ostream & operator<<(std::ostream &, Token::Type);

class Utf8Stream;

class TokenStream {
public:
	TokenStream(Utf8Stream &);

	void scan();

	Token token;
private:
	typedef void (TokenStream::*scanner)(void);

	scanner select_scanner(int);

	void invalid_token();
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

#endif
