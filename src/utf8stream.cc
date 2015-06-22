/*
   Copyright (c) 2015, Andreas Fett. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.
*/
#include <inttypes.h>

#include <cassert>

#include "utf8stream.h"

namespace jsonp {

Utf8Stream::Utf8Stream(const char *buf, size_t len)
:
	buf_(buf),
	len_(len),
	pos_(0),
	bad_(false),
	eof_(false),
	utf8_(),
	valid_(0)
{ }

Utf8Stream::State Utf8Stream::state() const
{
	if (bad_) {
		return SBAD;
	}

	if (eof_) {
		return SEOF;
	}

	return SGOOD;
}

int Utf8Stream::getc()
{
	if (bad_) {
		return SBAD;
	}

	if (pos_ == len_) {
		eof_ = true;
		return SEOF;
	}

	uint8_t c(buf_[pos_]);
	if (c == '\0') {
		bad_ = true;
		return SBAD;
	}

	if (valid_ <= pos_) {
		if ((bad_ = !utf8_.validate(c))) {
			return SBAD;
		}
		++valid_;
	}

	++pos_;
	return c;
}

void Utf8Stream::ungetc()
{
	if (pos_ != 0 && !bad_ && !eof_) {
		--pos_;
	}
}

}
