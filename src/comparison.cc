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

bool operator==(Null const&, Null const&)
{
	return true;
}

bool operator==(True const&, True const&)
{
	return true;
}

bool operator==(False const&, False const&)
{
	return true;
}

bool operator==(Number const& l, Number const& r)
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

bool operator==(String const& l, String const& r)
{
	return (&l == &r) || (l.value() == r.value());
}

bool operator==(Array const& l, Array const& r)
{
	return (&l == &r) || (l.elements() == r.elements());
}

bool operator==(Member const& l, Member const& r)
{
	return (&l == &r) || (l.key() == r.key() && l.value() == r.value());
}

bool operator==(Object const& l, Object const& r)
{
	return (&l == &r) || (l.members() == r.members());
}

bool operator==(Value const& l, Value const& r)
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
		return l.true_value() == r.true_value();
	case Value::TAG_FALSE:
		return l.false_value() == r.false_value();
	case Value::TAG_NULL:
		return l.null() == r.null();
	case Value::TAG_NUMBER:
		return l.number() == r.number();
	case Value::TAG_STRING:
		return l.string() == r.string();
	case Value::TAG_OBJECT:
		return l.object() == r.object();
	case Value::TAG_ARRAY:
		return l.array() == r.array();
	}

	return false;
}

}
