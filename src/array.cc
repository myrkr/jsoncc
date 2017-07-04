/*
   Copyright (c) 2015, 2016, Andreas Fett. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.
*/

#include <jsoncc.h>

namespace Json {

Array::Array()
:
	elements_()
{ }

Array::Array(Array const& o)
:
	elements_(o.elements_)
{ }

Array::Array(Array && o)
:
	elements_(std::move(o.elements_))
{ }

Array & Array::operator=(Array const& o)
{
	if (&o != this) {
		elements_ = o.elements_;
	}
	return *this;
}

Array & Array::operator=(Array && o)
{
	if (&o != this) {
		elements_ = std::move(o.elements_);
	}
	return *this;
}

Array & Array::operator<<(Value const& element)
{
	elements_.push_back(element);
	return *this;
}

std::vector<Value> Array::elements() const
{
	return elements_;
}

}
