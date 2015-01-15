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

namespace Json {

Value::Value()
:
	value_()
{ }

Value::Value(Value const& o)
:
	value_(o.value_)
{ }

Value & Value::operator=(Value const& o)
{
	if (&o != this) {
		value_ = o.value_;
	}
	return *this;
}

Value::Value(Null const& null)
{
	set(null);
}

Value::Value(True const& true_value)
{
	set(true_value);
}

Value::Value(False const& false_value)
{
	set(false_value);
}

Value::Value(Number const& number)
{
	set(number);
}

Value::Value(String const& string)
{
	set(string);
}

Value::Value(Object const& object)
{
	set(object);
}

Value::Value(Array const& array)
{
	set(array);
}

void Value::set(Null const& null)
{
	value_.set(null);
}

void Value::set(True const& true_value)
{
	value_.set(true_value);
}

void Value::set(False const& false_value)
{
	value_.set(false_value);
}

void Value::set(Number const& number)
{
	value_.set(number);
}

void Value::set(String const& string)
{
	value_.set(string);
}

void Value::set(Object const& object)
{
	value_.set(object);
}

void Value::set(Array const& array)
{
	value_.set(array);
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
