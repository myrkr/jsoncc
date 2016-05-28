/*
   Copyright (c) 2015, 2016, Andreas Fett. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.
*/
#include <jsoncc.h>
#include "utf8.h"

namespace Json {

class Utf8Stream {
public:
	enum State {
		SGOOD = 0,
		SEOF = -1,
		SBAD = -2,
	};

	Utf8Stream(const char *, size_t);
	State state() const;
	int getc(); // throws jsonp::Error
	void ungetc();
	Location location() const;
	void bad();

private:
	const char *buf_;
	size_t len_;
	size_t pos_;
	bool bad_;
	bool eof_;
	utf8validator utf8_;
	size_t valid_;
};

}
