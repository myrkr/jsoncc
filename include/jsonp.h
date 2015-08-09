/*
   Copyright (c) 2015, Andreas Fett
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
   FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
   OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef JSONP_H
#define JSONP_H

#include <cstddef>
#include <stdexcept>

namespace jsonp {

/* https://tools.ietf.org/html/rfc7159 */

class ParserImpl;

struct Location {
	size_t offs;
	size_t character;
	size_t line;

	Location(size_t = 0, size_t = 0, size_t = 0);
};

struct Error : public std::runtime_error {
	enum Type {
		OK = 0,            /* no error */
		STREAM_ZERO,       /* ascii zero found in source */
		UTF8_INVALID,      /* invalid utf8 in source */
		TOKEN_INVALID,     /* unrecognized token */
		LITERAL_INVALID,   /* bad literal */
		STRING_CTRL,       /* unescaped control char in string */
		STRING_QUOTE,      /* unterminated string (at eof) */
		ESCAPE_INVALID,    /* invalid escape char */
		UESCAPE_INVALID,   /* invalide unicode escape */
		UESCAPE_ZERO,      /* unicode escape yields zero */
		UESCAPE_SURROGATE, /* unicode escape yields surrogate */
		NUMBER_INVALID,    /* invalid number format */
		NUMBER_OVERFLOW,   /* number too long */
	} type;

	Location location;

	Error(Type = OK, Location = Location());
};

class Parser {
public:
	Parser();
	~Parser();

	void parse(char const *, size_t);
private:
	Parser(Parser const&); // = deleted;
	Parser & operator=(Parser const&); // = deleted;

	ParserImpl *impl_;
};

}
#endif
