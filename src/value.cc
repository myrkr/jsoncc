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

void ValueFactory<Null>::build(Null const& null, TaggedType & res)
{
	res.set(null);
}

void ValueFactory<True>::build(True const& true_value, TaggedType & res)
{
	res.set(true_value);
}

void ValueFactory<False>::build(False const& false_value, TaggedType & res)
{
	res.set(false_value);
}

void ValueFactory<String>::build(String const& string, TaggedType & res)
{
	res.set(string);
}

void ValueFactory<Array>::build(Array const& array, TaggedType & res)
{
	res.set(array);
}

void ValueFactory<Object>::build(Object const& object, TaggedType & res)
{
	res.set(object);
}

void ValueFactory<bool>::build(bool const& value, TaggedType & res)
{
	if (value) {
		res.set(True());
	} else {
		res.set(False());
	}
}

void ValueFactory<uint8_t>::build(uint8_t const& value, TaggedType & res)
{
	res.set(Number(value));
}

void ValueFactory<int8_t>::build(int8_t const& value, TaggedType & res)
{
	res.set(Number(value));
}

void ValueFactory<uint16_t>::build(uint16_t const& value, TaggedType & res)
{
	res.set(Number(value));
}

void ValueFactory<int16_t>::build(int16_t const& value, TaggedType & res)
{
	res.set(Number(value));
}

void ValueFactory<uint32_t>::build(uint32_t const& value, TaggedType & res)
{
	res.set(Number(value));
}

void ValueFactory<int32_t>::build(int32_t const& value, TaggedType & res)
{
	res.set(Number(value));
}

void ValueFactory<uint64_t>::build(uint64_t const& value, TaggedType & res)
{
	res.set(Number(value));
}

void ValueFactory<int64_t>::build(int64_t const& value, TaggedType & res)
{
	res.set(Number(value));
}

void ValueFactory<float>::build(float const& value, TaggedType & res)
{
	res.set(Number(value));
}

void ValueFactory<double>::build(double const& value, TaggedType & res)
{
	res.set(Number(value));
}

void ValueFactory<long double>::build(long double const& value, TaggedType & res)
{
	res.set(Number(value));
}

}
