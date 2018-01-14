/*
   Copyright (c) 2015 - 2017 Andreas Fett. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.
 */
#include <jsoncc.h>
#include <algorithm>

namespace Json {

Object::Object()
:
	members_()
{ }

Object::Object(std::initializer_list<Member> l)
:
	members_(l)
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

Object & Object::operator<<(Member && member)
{
	members_.push_back(std::move(member));
	return *this;
}

std::vector<Member> Object::members() const
{
	return members_;
}

Value Object::member(std::string const& key) const
{

	auto it(std::find_if(members_.begin(), members_.end(),
		[key](Member const& m) { return m.key().value() == key; }));
	return it != members_.end() ? it->value() : Value();
}

std::vector<Member>::const_iterator Object::begin() const
{
	return members_.begin();
}

std::vector<Member>::const_iterator Object::end() const
{
	return members_.end();
}

}
