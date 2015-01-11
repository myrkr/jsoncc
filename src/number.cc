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

}
