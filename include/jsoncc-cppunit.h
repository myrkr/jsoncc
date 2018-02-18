#ifndef JSONCC_CPPUNIT_H
#define JSONCC_CPPUNIT_H

#include <jsoncc.h>
#include <cppunit/TestAssert.h>

namespace Json {

template <typename T>
struct assertion_traits {
	static bool equal(T const& l, T const& r)
	{
		return Json::equal(l, r);
	}

	static std::string toString(T const& v)
	{
		std::stringstream os;
		os << v;
		return os.str();
	}
};

}

CPPUNIT_NS_BEGIN

template <>
struct assertion_traits<Json::Null> : public Json::assertion_traits<Json::Null>
{ };

template <>
struct assertion_traits<Json::True> : public Json::assertion_traits<Json::True>
{ };

template <>
struct assertion_traits<Json::False> : public Json::assertion_traits<Json::False>
{ };

template <>
struct assertion_traits<Json::Number> : public Json::assertion_traits<Json::Number>
{ };

template <>
struct assertion_traits<Json::String> : public Json::assertion_traits<Json::String>
{ };

template <>
struct assertion_traits<Json::Array> : public Json::assertion_traits<Json::Array>
{ };

template <>
struct assertion_traits<Json::Object> : public Json::assertion_traits<Json::Object>
{ };

template <>
struct assertion_traits<Json::Value> : public Json::assertion_traits<Json::Value>
{ };

CPPUNIT_NS_END

#endif
