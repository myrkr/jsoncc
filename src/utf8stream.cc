/*
   Copyright (c) 2015, Andreas Fett. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.
*/
#include <inttypes.h>

#include "utf8stream.h"

namespace jsonp {

Utf8Stream::Utf8Stream(const char *buf, size_t len)
:
	buf_(buf),
	len_(len),
	pos_(0),
	bad_(false)
{ }

Utf8Stream::State Utf8Stream::state() const
{
	if (bad_) {
		return SBAD;
	}

	if (len_ == pos_) {
		return SEOF;
	}

	return SGOOD;
}

int Utf8Stream::getc()
{
	State s(state());
	if (s != SGOOD) {
		return s;
	}

	return (uint8_t)buf_[pos_++];
}

void Utf8Stream::ungetc()
{
	if (pos_ != 0 && state() != SBAD) {
		--pos_;
	}
}

}
