/*
   Copyright (c) 2015, 2016, Andreas Fett. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.
 */
#include <jsoncc.h>
#include <cassert>

namespace Json {

Member::Member()
:
	key_(),
	value_()
{ }

Member::Member(Member const& o)
:
	key_(o.key_),
	value_(o.value_)
{ }

Member::Member(Member && o)
:
	key_(std::move(o.key_)),
	value_(std::move(o.value_))
{ }

Member::Member(std::string const& key, Value const& value)
:
	key_(key),
	value_(value)
{
	assert(!key.empty());
}

Member & Member::operator=(Member const& o)
{
	if (&o != this) {
		key_ = o.key_;
		value_ = o.value_;
	}
	return *this;
}

Member & Member::operator=(Member && o)
{
	if (&o != this) {
		key_ = std::move(o.key_);
		value_ = std::move(o.value_);
	}
	return *this;
}

String Member::key() const
{
	return key_;
}

Value Member::value() const
{
	return value_;
}

}
