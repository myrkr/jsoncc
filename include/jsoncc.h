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
#ifndef JSONCC_H
#define JSONCC_H

#include <stdint.h>

#include <sstream>
#include <vector>

namespace Json {
struct Null {};
struct True {};
struct False {};
class Number;
class String;
class Array;
class Member;
class Object;
class Value;
}

std::ostream & operator<<(std::ostream &, Json::Null const&);
std::ostream & operator<<(std::ostream &, Json::True const&);
std::ostream & operator<<(std::ostream &, Json::False const&);
std::ostream & operator<<(std::ostream &, Json::Number const&);
std::ostream & operator<<(std::ostream &, Json::String const&);
std::ostream & operator<<(std::ostream &, Json::Array const&);
std::ostream & operator<<(std::ostream &, Json::Member const&);
std::ostream & operator<<(std::ostream &, Json::Object const&);
std::ostream & operator<<(std::ostream &, Json::Value const&);

namespace Json {

class Number {
public:
	Number();
	Number(Number const&);
	Number(uint8_t);
	Number(int8_t);
	Number(uint16_t);
	Number(int16_t);
	Number(uint32_t);
	Number(int32_t);
	Number(uint64_t);
	Number(int64_t);
	Number(float);
	Number(double);
	Number(long double);

	Number & operator=(Number const&);

private:
	friend std::ostream & ::operator<<(std::ostream &, Number const&);

	enum Type {
		TYPE_INVALID,
		TYPE_INT,
		TYPE_UINT,
		TYPE_FP,
	} type_;

	union {
		uint64_t uint_;
		int64_t int_;
		long double float_;
	} value_;
};

class String {
public:
	String();
	String(String const&);
	String(std::string const&);
	String(const char *);

	String & operator=(String const&);

private:
	friend std::ostream & ::operator<<(std::ostream &, String const&);

	std::string value_;
};

class TaggedType {
public:
	enum Tag {
		TAG_INVALID = 0,
		TAG_NULL,
		TAG_TRUE,
		TAG_FALSE,
		TAG_NUMBER,
		TAG_STRING,
		TAG_OBJECT,
		TAG_ARRAY,
	};

	TaggedType();
	TaggedType(TaggedType const&);
	TaggedType & operator=(TaggedType const&);

	~TaggedType();

	void set(Null const&);
	void set(True const&);
	void set(False const&);
	void set(Number const&);
	void set(String const&);
	void set(Object const&);
	void set(Array const&);

	Tag tag() const;

	True const& true_value() const;
	False const& false_value() const;
	Null const& null() const;
	Number const& number() const;
	String const& string() const;
	Object const& object() const;
	Array const& array() const;

private:
	void clone(TaggedType const&);
	void clear();

	Tag tag_;
	static True true_value_;
	static False False_value_;
	static Null Null_value_;

	union Type {
		True *true_;
		False *false_;
		Null *null_;
		Number *number_;
		String *string_;
		Object *object_;
		Array *array_;
	} type_;
};

template<typename T> struct ValueFactory;

class Value {
public:
	Value();

	template <typename T>
	Value(T const& value)
	:
		value_()
	{
		ValueFactory<T>::build(value, *this);
	}

	Value(Null const&);
	Value(True const&);
	Value(False const&);
	Value(Number const&);
	Value(String const&);
	Value(Object const&);
	Value(Array const&);

	Value(Value const&);
	Value & operator=(Value const&);

	void set(Null const&);
	void set(True const&);
	void set(False const&);
	void set(Number const&);
	void set(String const&);
	void set(Object const&);
	void set(Array const&);

private:
	friend std::ostream & ::operator<<(std::ostream &, Value const&);

	TaggedType value_;
};

class Member {
public:
	Member();
	Member(Member const&);
	Member(std::string const&, Value const&);

	Member & operator=(Member const&);

private:
	friend std::ostream & ::operator<<(std::ostream &, Member const&);

	String key_;
	Value value_;
};

class Object {
public:
	Object();
	Object(Object const&);

	Object & operator=(Object const&);
	Object & operator<<(Member const&);

private:
	friend std::ostream & ::operator<<(std::ostream &, Object const&);

	typedef std::vector<Member> container;
	typedef container::const_iterator const_iterator;
	container member_;
};

class Array {
public:
	Array();
	Array(Array const&);

	Array & operator=(Array const&);
	Array & operator<<(Value const&);

private:
	friend std::ostream & ::operator<<(std::ostream &, Array const&);

	typedef std::vector<Value> container;
	typedef container::const_iterator const_iterator;
	container element_;
};

template<> struct ValueFactory<bool>        { static void build(bool        const&, Value &); };
template<> struct ValueFactory<uint8_t>     { static void build(uint8_t     const&, Value &); };
template<> struct ValueFactory<int8_t>      { static void build(int8_t      const&, Value &); };
template<> struct ValueFactory<uint16_t>    { static void build(uint16_t    const&, Value &); };
template<> struct ValueFactory<int16_t>     { static void build(int16_t     const&, Value &); };
template<> struct ValueFactory<uint32_t>    { static void build(uint32_t    const&, Value &); };
template<> struct ValueFactory<int32_t>     { static void build(int32_t     const&, Value &); };
template<> struct ValueFactory<uint64_t>    { static void build(uint64_t    const&, Value &); };
template<> struct ValueFactory<int64_t>     { static void build(int64_t     const&, Value &); };
template<> struct ValueFactory<float>       { static void build(float       const&, Value &); };
template<> struct ValueFactory<double>      { static void build(double      const&, Value &); };
template<> struct ValueFactory<long double> { static void build(long double const&, Value &); };

template<typename E> struct ValueFactory<std::vector<E> > {
	typedef std::vector<E> container;

	static void build(container const& v, Value & res)
	{
		Json::Array a;
		typename container::const_iterator it(v.begin());
		for (; it != v.end(); ++it) {
			a << *it;
		}
		res.set(a);
	}
};

template<typename T> struct ValueFactory {
	static void build(T const& v, Value & res)
	{
		std::stringstream ss;
		ss << v;
		res.set(ss.str());
	}
};

}

#endif
