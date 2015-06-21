/*
   Copyright (c) 2015, Andreas Fett. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.
*/

#include <inttypes.h>

#include <stdexcept>
#include <ostream>
#include <cassert>

#include "token-stream.h"
#include "utf8stream.h"

namespace {

bool is_ws(int c)
{
	return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

}

namespace jsonp {

// LCOV_EXCL_START
std::ostream & operator<<(std::ostream & os, Token::Type type)
{
#define CASE_TOKEN_TYPE(name) case name: os << # name; break
	switch (type) {
	CASE_TOKEN_TYPE(Token::INVALID);
	CASE_TOKEN_TYPE(Token::BEGIN_ARRAY);
	CASE_TOKEN_TYPE(Token::END_ARRAY);
	CASE_TOKEN_TYPE(Token::BEGIN_OBJECT);
	CASE_TOKEN_TYPE(Token::END_OBJECT);
	CASE_TOKEN_TYPE(Token::NAME_SEPARATOR);
	CASE_TOKEN_TYPE(Token::VALUE_SEPARATOR);
	CASE_TOKEN_TYPE(Token::TRUE_LITERAL);
	CASE_TOKEN_TYPE(Token::FALSE_LITERAL);
	CASE_TOKEN_TYPE(Token::NULL_LITERAL);
	CASE_TOKEN_TYPE(Token::STRING);
	CASE_TOKEN_TYPE(Token::NUMBER);
	}
#undef CASE_TOKEN_TYPE
	return os;
}
// LCOV_EXCL_STOP

TokenStream::TokenStream(Utf8Stream & stream)
:
	stream_(stream)
{ }

void TokenStream::scan()
{
	token.reset();

	int c;
	do {
		c = stream_.getc();
	} while (is_ws(c));

	if (c == int(Utf8Stream::SEOF)) {
		return;
	}

	if (stream_.state() == Utf8Stream::SBAD) {
		return;
	}

	try {
		(this->*select_scanner(c))();
	} catch (std::runtime_error const& e) {
		throw;
	}
}

TokenStream::scanner TokenStream::select_scanner(int c)
{
	scanner res(0);

	switch (c) {
	case '[': case '{': case ']':
	case '}': case ':': case ',':
		res = &TokenStream::scan_structural;
		break;
	case 't':
		res = &TokenStream::scan_true;
		break;
	case 'n':
		res = &TokenStream::scan_null;
		break;
	case 'f':
		res = &TokenStream::scan_false;
		break;
	case '"':
		res = &TokenStream::scan_string;
		break;
	case '-':
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		c = '0';
		res = &TokenStream::scan_number;
		break;
	default:
		c = 0;
		res = &TokenStream::invalid_token;
		break;
	}

	token.type = Token::Type(c);
	return res;
}

void TokenStream::invalid_token() { throw std::runtime_error("invalid token"); }
void TokenStream::scan_structural() { /* NOOP */ }
void TokenStream::scan_true() { scan_literal("true"); }
void TokenStream::scan_false() { scan_literal("false"); }
void TokenStream::scan_null() { scan_literal("null"); }

void TokenStream::scan_literal(const char *literal)
{
	for (const char *p(&literal[1]); *p; p++) {
		if (stream_.getc() != *p) {
			throw std::runtime_error(literal);
		}
	}
}

void TokenStream::scan_string()
{
	// TODO
}

void TokenStream::scan_number()
{
	stream_.ungetc();

	int c(stream_.getc());
	bool minus(c == '-');
	if (minus) {
		c = stream_.getc();
	}

	uint64_t mantissa(0);
	size_t digits(0);
	size_t point(0);
	bool done(false);
	for (; !done; c = stream_.getc()) {
		switch (c) {
		case '.':
			if (!point) {
				point = digits;
			} else {
				throw std::runtime_error("invalid decimal point");
			}
			break;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			mantissa = mantissa * 10 + (c - '0');
			if (++digits > 1 && !mantissa) {
				throw std::runtime_error("no leading zeros please");
			}
			break;
		default:
			done = true;
			break;
		}
	}

	bool eminus(false);
	uint64_t exponent(0);
	if (c == 'e' || c == 'E') {
		c = stream_.getc();
		if (c == '+' || c == '-') {
			eminus = c == '-';
			c = stream_.getc();
		}

		while (c >= '0' && c <= '9') {
			exponent = exponent * 10 + (c - '0');
			c = stream_.getc();
		}
	}

	if (eminus) {
	}
}

}
