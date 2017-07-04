/*
   Copyright (c) 2015, 2016, Andreas Fett. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.
 */
#include <jsoncc.h>
#include <cassert>

namespace Json {

Number::Number()
:
	type_(TYPE_INVALID),
	value_()
{ }

Number::Number(Number const& o)
:
	type_(o.type_),
	value_(o.value_)
{ }

Number::Number(Number && o)
:
	type_(std::move(o.type_)),
	value_(std::move(o.value_))
{ }

Number::Number(uint8_t value)
:
	type_(TYPE_UINT),
	value_()
{
	value_.uint_ = value;
}

Number::Number(int8_t value)
:
	type_(TYPE_INT),
	value_()
{
	value_.int_ = value;
}

Number::Number(uint16_t value)
:
	type_(TYPE_UINT),
	value_()
{
	value_.uint_ = value;
}

Number::Number(int16_t value)
:
	type_(TYPE_INT),
	value_()
{
	value_.int_ = value;
}

Number::Number(uint32_t value)
:
	type_(TYPE_UINT),
	value_()
{
	value_.uint_ = value;
}

Number::Number(int32_t value)
:
	type_(TYPE_INT),
	value_()
{
	value_.int_ = value;
}

Number::Number(uint64_t value)
:
	type_(TYPE_UINT),
	value_()
{
	value_.uint_ = value;
}

Number::Number(int64_t value)
:
	type_(TYPE_INT),
	value_()
{
	value_.int_ = value;
}

Number::Number(float value)
:
	type_(TYPE_FP),
	value_()
{
	value_.float_ = value;
}

Number::Number(double value)
:
	type_(TYPE_FP),
	value_()
{
	value_.float_ = value;
}

Number::Number(long double value)
:
	type_(TYPE_FP),
	value_()
{
	value_.float_ = value;
}

Number & Number::operator=(Number const& o)
{
	if (&o != this) {
		type_ = o.type_;
		value_ = o.value_;
	}
	return *this;
}

Number & Number::operator=(Number && o)
{
	if (&o != this) {
		type_ = std::move(o.type_);
		value_ = std::move(o.value_);
	}
	return *this;
}

Number::Type Number::type() const
{
	return type_;
}

uint64_t Number::uint_value() const
{
	assert(type_ == TYPE_UINT);
	return value_.uint_;
}

int64_t Number::int_value() const
{
	assert(type_ == TYPE_INT);
	return value_.int_;
}

long double Number::fp_value() const
{
	assert(type_ == TYPE_FP);
	return value_.float_;
}

}
