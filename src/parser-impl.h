/*
   Copyright (c) 2015, Andreas Fett. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.
*/
#ifndef RAD_PARSER_IMPL_H
#define RAD_PARSER_IMPL_H

#include <cstddef>

namespace jsonp {

class ParserImpl {
public:
	void parse(char const *, size_t);
};

}

#endif
