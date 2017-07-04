/*
   Copyright (c) 2015, 2016, Andreas Fett. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.
 */
#include <jsoncc.h>

namespace Json {

Object::Object()
:
	members_()
{ }

Object::Object(Object const& o)
:
	members_(o.members_)
{ }

Object::Object(Object && o)
:
	members_(std::move(o.members_))
{ }

Object & Object::operator=(Object const& o)
{
	if (&o != this) {
		members_ = o.members_;
	}
	return *this;
}

Object & Object::operator=(Object && o)
{
	if (&o != this) {
		members_ = std::move(o.members_);
	}
	return *this;
}

Object & Object::operator<<(Member const& member)
{
	members_.push_back(member);
	return *this;
}

std::vector<Member> Object::members() const
{
	return members_;
}

Value Object::member(std::string const& key) const
{
	for (std::vector<Member>::const_iterator it = members_.begin(); it != members_.end(); ++it) {
		if (it->key().value() == key) {
			return it->value();
		}
	}
	return Value();
}

}
