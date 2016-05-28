/*
   Copyright (c) 2015, 2016, Andreas Fett. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.
*/
#include <jsoncc.h>

namespace Json {

class ParserImpl {
public:
	Value parse(char const *, size_t);
	Value parse(char const *, size_t, Error &);
};

}
