#include <cppunit/extensions/HelperMacros.h>
#include <jsoncc.h>

#include <jsoncc-cppunit.h>
#include "clang-helpers.h"

namespace unittests {
namespace json {
namespace object {

class test : public CppUnit::TestCase {
public:
	test();
	void setUp();
	void tearDown();

private:
	void test_empty();
	void test_simple();
	void test_nested();
	void test_nested_noindent();
	void test_member();
	void test_unknown_member();
	void test_equality();
	void test_iterators();
	void test_list_initialization();
	void test_move();

	CPPUNIT_TEST_SUITE(test);
	CPPUNIT_TEST(test_empty);
	CPPUNIT_TEST(test_simple);
	CPPUNIT_TEST(test_nested);
	CPPUNIT_TEST(test_nested_noindent);
	CPPUNIT_TEST(test_member);
	CPPUNIT_TEST(test_unknown_member);
	CPPUNIT_TEST(test_equality);
	CPPUNIT_TEST(test_iterators);
	CPPUNIT_TEST(test_list_initialization);
	CPPUNIT_TEST(test_move);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(test);

test::test()
{ }

void test::setUp()
{ }

void test::tearDown()
{ }

void test::test_empty()
{
	std::stringstream ss;
	ss << Json::Object();
	CPPUNIT_ASSERT_EQUAL(std::string("{}"), ss.str());
}

void test::test_simple()
{
	std::stringstream ss;
	Json::Object o;
	o << Json::Member("number", 5);
	o << Json::Member("bool", true);
	o << Json::Member("string", "Foo");
	o << Json::Member("array", Json::Array());
	o << Json::Member("whatever", Json::Null());
	ss << o;

	std::string expected(
		"{\n"
		"	\"number\": 5,\n"
		"	\"bool\": true,\n"
		"	\"string\": \"Foo\",\n"
		"	\"array\": [],\n"
		"	\"whatever\": null\n"
		"}"
	);

	CPPUNIT_ASSERT_EQUAL(expected, ss.str());
}

void test::test_nested()
{
	std::stringstream ss;
	Json::Object o1;
	o1 << Json::Member("foo", true);
	Json::Object o2;
	o2 << Json::Member("bar", 5);
	Json::Object o;
	o << Json::Member("1", o1);
        o << Json::Member("2", o2);
        o << Json::Member("3", Json::Object());
	ss << o;

	std::string expected(
		"{\n"
		"	\"1\": {\n"
		"		\"foo\": true\n"
		"	},\n"
		"	\"2\": {\n"
		"		\"bar\": 5\n"
		"	},\n"
		"	\"3\": {}\n"
		"}"
	);

	CPPUNIT_ASSERT_EQUAL(expected, ss.str());
}

void test::test_nested_noindent()
{
	std::stringstream ss;
	Json::Object o1;
	o1 << Json::Member("foo", true);
	Json::Object o2;
	o2 << Json::Member("bar", 5);
	Json::Object o;
	o << Json::Member("1", o1);
	o << Json::Member("2", o2);
	o << Json::Member("3", Json::Object());
	ss << Json::noindent << o;

	std::string expected(
		"{\"1\": {\"foo\": true}, \"2\": {\"bar\": 5}, \"3\": {}}"
	);

	CPPUNIT_ASSERT_EQUAL(expected, ss.str());
}

void test::test_member()
{
	Json::Object o;
	o << Json::Member("foo", true);
	o << Json::Member("bar", 5);

	CPPUNIT_ASSERT_EQUAL(Json::Value(true), o.member("foo"));
	CPPUNIT_ASSERT_EQUAL(Json::Value(5), o.member("bar"));
}

void test::test_unknown_member()
{
	Json::Object o;
	o << Json::Member("foo", true);

	CPPUNIT_ASSERT_EQUAL(Json::Value(true), o.member("foo"));
	CPPUNIT_ASSERT_EQUAL(Json::Value(), o.member("bar"));
}

void test::test_equality()
{
	Json::Object o1;
	CPPUNIT_ASSERT_EQUAL(o1, o1);
	o1 << Json::Member("number", 5);
	o1 << Json::Member("bool", true);
	o1 << Json::Member("string", "Foo");
	o1 << Json::Member("array", Json::Array());
	o1 << Json::Member("whatever", Json::Null());

	CPPUNIT_ASSERT_EQUAL(o1, o1);
	Json::Object o2(o1);
	CPPUNIT_ASSERT_EQUAL(o1, o2);

	CPPUNIT_ASSERT(!equal(o1, Json::Object()));

	Json::Object o3;
	o3 << Json::Member("number", 5);
	o3 << Json::Member("bool", true);
	o3 << Json::Member("string", "Foo");
	o3 << Json::Member("array", Json::Array());
	CPPUNIT_ASSERT(!equal(o1, o3));

	// CAVEAT!
	Json::Object o4;
	o3 << Json::Member("whatever", Json::Null());
	o3 << Json::Member("array", Json::Array());
	o3 << Json::Member("string", "Foo");
	o3 << Json::Member("bool", true);
	o3 << Json::Member("number", 5);
	CPPUNIT_ASSERT(!equal(o1, o4));

	Json::Object o5;
	o5 = o4;
	CPPUNIT_ASSERT_EQUAL(o5, o4);

	DONT_WARN_ABOUT_SELFASSIGNMENT;
	o5 = o5;
	WARN_ABOUT_SELFASSIGNMENT;
	CPPUNIT_ASSERT_EQUAL(o5, o4);
}

void test::test_iterators()
{
	Json::Object o;
	o << Json::Member("foo", true);
	o << Json::Member("bar", 5);
	o << Json::Member("baz", "bla");

	for (auto const& m: o) {
		if (m.key().value() == "foo") {
			CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_TRUE, m.value().tag());
		} else if (m.key().value() == "bar") {
			CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, m.value().tag());
		} else if (m.key().value() == "baz") {
			CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_STRING, m.value().tag());
		} else {
			CPPUNIT_ASSERT(false);
		}
	}
}

void test::test_list_initialization()
{
	Json::Object o1{
		Json::Member("foo", true),
		Json::Member("bar", 5),
		Json::Member("baz", "bla"),
	};

	for (auto const& m: o1) {
		if (m.key().value() == "foo") {
			CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_TRUE, m.value().tag());
		} else if (m.key().value() == "bar") {
			CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, m.value().tag());
		} else if (m.key().value() == "baz") {
			CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_STRING, m.value().tag());
		} else {
			CPPUNIT_ASSERT(false);
		}
	}

	Json::Object o2{ };
	CPPUNIT_ASSERT(o2.members().empty());

	Json::Object o3{
		{"foo", true},
		{"bar", 5},
		{"baz", "bla"}
	};

	for (auto const& m: o3) {
		if (m.key().value() == "foo") {
			CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_TRUE, m.value().tag());
		} else if (m.key().value() == "bar") {
			CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, m.value().tag());
		} else if (m.key().value() == "baz") {
			CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_STRING, m.value().tag());
		} else {
			CPPUNIT_ASSERT(false);
		}
	}
}

void test::test_move()
{
	Json::Object o1{
		Json::Member("foo", true),
		Json::Member("bar", 5),
		Json::Member("baz", "bla"),
	};

	Json::Object o2(std::move(o1));
	CPPUNIT_ASSERT(o1.members().empty());

	for (auto const& m: o2) {
		if (m.key().value() == "foo") {
			CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_TRUE, m.value().tag());
		} else if (m.key().value() == "bar") {
			CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, m.value().tag());
		} else if (m.key().value() == "baz") {
			CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_STRING, m.value().tag());
		} else {
			CPPUNIT_ASSERT(false);
		}
	}

	Json::Object o3;
	o3 = std::move(o2);
	CPPUNIT_ASSERT(o2.members().empty());

	for (auto const& m: o3) {
		if (m.key().value() == "foo") {
			CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_TRUE, m.value().tag());
		} else if (m.key().value() == "bar") {
			CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, m.value().tag());
		} else if (m.key().value() == "baz") {
			CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_STRING, m.value().tag());
		} else {
			CPPUNIT_ASSERT(false);
		}
	}
}

}}}
