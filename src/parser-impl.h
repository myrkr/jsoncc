/*
   Copyright (c) 2015, Andreas Fett. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.
*/
#include <cstddef>

namespace jsonp {

class TokenStream;

class ParserImpl {
public:
	void parse(char const *, size_t);

private:

	void parse_document(TokenStream &);
};

}
