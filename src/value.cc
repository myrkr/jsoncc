/*
   Copyright (c) 2015, Andreas Fett
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
   FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
   OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <jsoncc.h>
#include <cstring>
#include <cassert>

namespace Json {

static True TrueValue;
static False FalseValue;
static Null NullValue;

Value::Value()
:
	tag_(TAG_INVALID)
{
	memset(&type_, 0, sizeof(Type));
}

Value::Value(Value const& o)
:
	tag_(TAG_INVALID)
{
	clone(o);
}

Value & Value::operator=(Value const& o)
{
	if (&o != this) {
		clear();
		clone(o);
	}
	return *this;
}

Value::~Value()
{
	clear();
}

void Value::clone(Value const& o)
{
	assert(&o != this);

	tag_ = o.tag_;

	switch (tag_) {
	case TAG_INVALID:
		break;
	case TAG_TRUE:
		assert(o.type_.true_);
		type_.true_ = o.type_.true_;
		break;
	case TAG_FALSE:
		assert(o.type_.false_);
		type_.false_ = o.type_.false_;
		break;
	case TAG_NULL:
		assert(o.type_.null_);
		type_.null_ = o.type_.null_;
		break;
	case TAG_NUMBER:
		assert(o.type_.number_);
		type_.number_ = new Number(*o.type_.number_);
		break;
	case TAG_STRING:
		assert(o.type_.string_);
		type_.string_ = new String(*o.type_.string_);
		break;
	case TAG_OBJECT:
		assert(o.type_.object_);
		type_.object_ = new Object(*o.type_.object_);
		break;
	case TAG_ARRAY:
		assert(o.type_.array_);
		type_.array_ = new Array(*o.type_.array_);
		break;
	}
}

void Value::clear()
{
	switch (tag_) {
	case TAG_INVALID:
		return;
	case TAG_TRUE:
	case TAG_FALSE:
	case TAG_NULL:
		break;
	case TAG_NUMBER:
		assert(type_.number_);
		delete type_.number_;
		break;
	case TAG_STRING:
		assert(type_.string_);
		delete type_.string_;
		break;
	case TAG_OBJECT:
		assert(type_.object_);
		delete type_.object_;
		break;
	case TAG_ARRAY:
		assert(type_.array_);
		delete type_.array_;
		break;
	}

	tag_ = TAG_INVALID;
	memset(&type_, 0, sizeof(Type));
}

Value::Value(Null const& null)
:
	tag_(TAG_INVALID)
{
	set(null);
}

Value::Value(True const& true_value)
:
	tag_(TAG_INVALID)
{
	set(true_value);
}

Value::Value(False const& false_value)
:
	tag_(TAG_INVALID)
{
	set(false_value);
}

Value::Value(Number const& number)
:
	tag_(TAG_INVALID)
{
	set(number);
}

Value::Value(String const& string)
:
	tag_(TAG_INVALID)
{
	set(string);
}

Value::Value(Object const& object)
:
	tag_(TAG_INVALID)
{
	set(object);
}

Value::Value(Array const& array)
:
	tag_(TAG_INVALID)
{
	set(array);
}

void Value::set(Null const&)
{
	clear();
	tag_ = TAG_NULL;
	type_.null_ = &NullValue;
}

void Value::set(True const&)
{
	clear();
	tag_ = TAG_TRUE;
	type_.true_ = &TrueValue;
}

void Value::set(False const&)
{
	clear();
	tag_ = TAG_FALSE;
	type_.false_ = &FalseValue;
}

void Value::set(Number const& number)
{
	clear();
	tag_ = TAG_NUMBER;
	type_.number_ = new Number(number);
}

void Value::set(String const& string)
{
	clear();
	tag_ = TAG_STRING;
	type_.string_ = new String(string);
}

void Value::set(Object const& object)
{
	clear();
	tag_ = TAG_OBJECT;
	type_.object_ = new Object(object);
}

void Value::set(Array const& array)
{
	clear();
	tag_ = TAG_ARRAY;
	type_.array_ = new Array(array);
}

Value::Tag Value::tag() const
{
	return tag_;
}

Null const& Value::null() const
{
	assert(tag_ == TAG_NULL);
	assert(type_.null_);
	return *type_.null_;
}

True const& Value::true_value() const
{
	assert(tag_ == TAG_TRUE);
	assert(type_.true_);
	return *type_.true_;
}

False const& Value::false_value() const
{
	assert(tag_ == TAG_FALSE);
	assert(type_.false_);
	return *type_.false_;
}

Number const& Value::number() const
{
	assert(tag_ == TAG_NUMBER);
	assert(type_.number_);
	return *type_.number_;
}

String const& Value::string() const
{
	assert(tag_ == TAG_STRING);
	assert(type_.string_);
	return *type_.string_;
}

Array const& Value::array() const
{
	assert(tag_ == TAG_ARRAY);
	assert(type_.array_);
	return *type_.array_;
}

Object const& Value::object() const
{
	assert(tag_ == TAG_OBJECT);
	assert(type_.object_);
	return *type_.object_;
}

void ValueFactory<bool>::build(bool const& value, Value & res)
{
	if (value) {
		res.set(True());
	} else {
		res.set(False());
	}
}

void ValueFactory<uint8_t>::build(uint8_t const& value, Value & res)
{
	res.set(Number(value));
}

void ValueFactory<int8_t>::build(int8_t const& value, Value & res)
{
	res.set(Number(value));
}

void ValueFactory<uint16_t>::build(uint16_t const& value, Value & res)
{
	res.set(Number(value));
}

void ValueFactory<int16_t>::build(int16_t const& value, Value & res)
{
	res.set(Number(value));
}

void ValueFactory<uint32_t>::build(uint32_t const& value, Value & res)
{
	res.set(Number(value));
}

void ValueFactory<int32_t>::build(int32_t const& value, Value & res)
{
	res.set(Number(value));
}

void ValueFactory<uint64_t>::build(uint64_t const& value, Value & res)
{
	res.set(Number(value));
}

void ValueFactory<int64_t>::build(int64_t const& value, Value & res)
{
	res.set(Number(value));
}

void ValueFactory<float>::build(float const& value, Value & res)
{
	res.set(Number(value));
}

void ValueFactory<double>::build(double const& value, Value & res)
{
	res.set(Number(value));
}

void ValueFactory<long double>::build(long double const& value, Value & res)
{
	res.set(Number(value));
}

}
