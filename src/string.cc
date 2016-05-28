/*
   Copyright (c) 2015, 2016, Andreas Fett. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.
 */
#include <jsoncc.h>

namespace Json {

String::String()
:
	value_()
{ }

String::String(String const& o)
:
	value_(o.value_)
{ }

String::String(std::string const& value)
:
	value_(value)
{ }

String::String(const char *value)
:
	value_(value)
{ }

String & String::operator=(String const& o)
{
	if (&o != this) {
		value_ = o.value_;
	}
	return *this;
}

std::string String::value() const
{
	return value_;
}

}
