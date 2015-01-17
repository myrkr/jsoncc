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
#include <cassert>
#include <iomanip>

namespace {

class indent : public std::streambuf {
public:
	explicit indent(std::streambuf*);
	explicit indent(std::ostream &, std::string indent = "\t");
	virtual ~indent();

protected:
	virtual int overflow(int);

private:
	std::string indent_;
	std::streambuf* dest_;
	bool line_start_;
	std::ostream* owner_;
};

indent::indent(std::streambuf* dest)
:
	dest_(dest),
	line_start_(true),
	owner_(NULL)
{ }

indent::indent(std::ostream& dest, std::string indent)
:
	indent_(indent),
	dest_(dest.rdbuf()),
	line_start_(true),
	owner_(&dest)
{
	owner_->rdbuf(this);
}

indent::~indent()
{
	if (owner_ != NULL) {
		owner_->rdbuf(dest_);
	}
}

int indent::overflow(int ch)
{
	if (line_start_ && ch != '\n') {
		dest_->sputn(indent_.c_str(), indent_.size());
	}
	line_start_ = (ch == '\n');
	return dest_->sputc(ch);
}

std::ostream & uescape(std::ostream & os)
{
	return os << "\\u" << std::setw(4) << std::hex << std::setfill('0');
}

std::ostream & quote(std::ostream & os, std::string const& in)
{
	os << '"';
	std::string::const_iterator it(in.begin());
	for (; it != in.end(); ++it) {
		switch (*it) {
		case 0x00: case 0x01: case 0x02: case 0x03: case 0x04:
		case 0x05: case 0x06: case 0x07: /* 0x08 */ /* 0x09 */
		/* 0x0a */ case 0x0b: /* 0x0c */ /* 0x0d */ case 0x0e:
		case 0x0f: case 0x10: case 0x11: case 0x12: case 0x13:
		case 0x14: case 0x15: case 0x16: case 0x17: case 0x18:
		case 0x19: case 0x1a: case 0x1b: case 0x1c: case 0x1d:
		case 0x1e: case 0x1f:
			os << uescape << static_cast<int>(*it);
			break;
		case 0x08:
			os << "\\b";
			break;
		case 0x09:
			os << "\\t";
			break;
		case 0x0a:
			os << "\\n";
			break;
		case 0x0c:
			os << "\\f";
			break;
		case 0x0d:
			os << "\\r";
			break;
		case '"':
			os << "\\\"";
			break;
		case '\\':
			os << "\\\\";
			break;
		default:
			os << *it;
		}
	}
	return os << '"';
}

template <typename C>
std::ostream & stream_container(std::ostream & os, const char delim[3], C const& c)
{
	if (c.empty()) {
		return os << delim;
	}

	os << delim[0] << "\n";
	{
		indent in(os);
		std::string sep;
		typename C::const_iterator it(c.begin());
		for (; it != c.end(); ++it) {
			os << sep << *it;
			sep = ",\n";
		}
	}
	return os << "\n" << delim[1] ;
}

}

std::ostream & operator<<(std::ostream & os, Json::Null const&)
{
	return os << "null";
}

std::ostream & operator<<(std::ostream & os, Json::True const&)
{
	return os << "true";
}

std::ostream & operator<<(std::ostream & os, Json::False const&)
{
	return os << "false";
}

std::ostream & operator<<(std::ostream & os, Json::Number const& number)
{
	switch (number.type_) {
	case Json::Number::TYPE_INVALID:
		assert(false);
		break;
	case Json::Number::TYPE_INT:
		return os << number.value_.int_;
	case Json::Number::TYPE_UINT:
		return os << number.value_.uint_;
	case Json::Number::TYPE_FP:
		return os << std::fixed << number.value_.float_;
	}

	return os;
}

std::ostream & operator<<(std::ostream & os, Json::String const& string)
{
	return ::quote(os, string.value_);
}

std::ostream & operator<<(std::ostream & os, Json::Array const& array)
{
	return stream_container(os, "[]", array.element_);
}

std::ostream & operator<<(std::ostream & os, Json::Member const& member)
{
	return os << member.key_ << ": " << member.value_;
}

std::ostream & operator<<(std::ostream & os, Json::Object const& object)
{
	return stream_container(os, "{}", object.member_);
}

std::ostream & operator<<(std::ostream & os, Json::Value const& value)
{
	switch (value.value_.tag()) {
	case Json::TaggedType::TAG_INVALID:
		break;
	case Json::TaggedType::TAG_TRUE:
		return os << value.value_.true_value();
	case Json::TaggedType::TAG_FALSE:
		return os << value.value_.false_value();
	case Json::TaggedType::TAG_NULL:
		return os << value.value_.null();
	case Json::TaggedType::TAG_NUMBER:
		return os << value.value_.number();
	case Json::TaggedType::TAG_STRING:
		return os << value.value_.string();
	case Json::TaggedType::TAG_OBJECT:
		return os << value.value_.object();
	case Json::TaggedType::TAG_ARRAY:
		return os << value.value_.array();
	}

	return os;
}
