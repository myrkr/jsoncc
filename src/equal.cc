/*
   Copyright (c) 2015 - 2018, Andreas Fett. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.
*/

#include <algorithm>
#include <jsoncc.h>

namespace Json {

bool equal(Null const&, Null const&)
{
	return true;
}

bool equal(True const&, True const&)
{
	return true;
}

bool equal(False const&, False const&)
{
	return true;
}

bool equal(Number const& l, Number const& r)
{
	if (&l == &r) {
		return true;
	}

	if (l.type() != r.type()) {
		return false;
	}

	switch (l.type()) {
	case Number::TYPE_INVALID:
		return true;
	case Number::TYPE_UINT:
		return l.uint_value() == r.uint_value();
	case Number::TYPE_INT:
		return l.int_value() == r.int_value();
	case Number::TYPE_FP:
		return l.fp_value() == r.fp_value();
	}

	return false;
}

bool equal(String const& l, String const& r)
{
	return (&l == &r) || (l.value() == r.value());
}

bool equal(Array const& l, Array const& r)
{
	if (&l == &r) {
		return true;
	}

	if (l.size() != r.size()) {
		return false;
	}

	using value_eq = bool(*)(Value const&, Value const&);
	return equal(
		std::begin(l), std::end(l),
		std::begin(r), static_cast<value_eq>(equal));
}

bool equal(Member const& l, Member const& r)
{
	return (&l == &r) || (equal(l.key(), r.key()) && equal(l.value(), r.value()));
}

bool equal(Object const& l, Object const& r)
{
	if (&l == &r) {
		return true;
	}

	if (l.size() != r.size()) {
		return false;
	}

	using member_eq = bool(*)(Member const&, Member const&);
	return is_permutation(
		std::begin(l), std::end(l),
		std::begin(r), static_cast<member_eq>(equal));
}

bool equal(Value const& l, Value const& r)
{
	if (&l == &r) {
		return true;
	}

	if (l.tag() != r.tag()) {
		return false;
	}

	switch (l.tag()) {
	case Value::TAG_INVALID:
		return true;
	case Value::TAG_TRUE:
		return equal(l.true_value(), r.true_value());
	case Value::TAG_FALSE:
		return equal(l.false_value(), r.false_value());
	case Value::TAG_NULL:
		return equal(l.null(), r.null());
	case Value::TAG_NUMBER:
		return equal(l.number(), r.number());
	case Value::TAG_STRING:
		return equal(l.string(), r.string());
	case Value::TAG_OBJECT:
		return equal(l.object(), r.object());
	case Value::TAG_ARRAY:
		return equal(l.array(), r.array());
	}

	return false;
}

}
