/*
   Copyright (c) 2015, 2016, Andreas Fett. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.
*/

#include <jsoncc.h>
#include "parser-impl.h"

namespace Json {

Parser::Parser()
:
	impl_(new ParserImpl())
{ }

Parser::~Parser()
{ }

Value Parser::parse(char const * data, size_t size)
{
	return impl_->parse(data, size);
}

Value Parser::parse(char const * data, size_t size, Error & err)
{
	try {
		parse(data, size);
	} catch (Error & e) {
		err = e;
	}
	return Value();
}

}
