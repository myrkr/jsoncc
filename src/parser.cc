/*
   Copyright (c) 2015, Andreas Fett. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.
*/

#include <jsonp.h>
#include "parser-impl.h"

namespace jsonp {

Parser::Parser()
:
	impl_(new ParserImpl())
{ }

Parser::~Parser()
{
	delete impl_;
}

void Parser::parse(char const * data, size_t size)
{
	impl_->parse(data, size);
}

}
