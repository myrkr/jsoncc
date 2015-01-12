#include <cppunit/extensions/HelperMacros.h>
#include <jsoncc.h>

namespace unittests {
namespace json {

class test : public CppUnit::TestCase {
public:
	test();
	void setUp();
	void tearDown();

private:
	void test_null();
	void test_true();
	void test_false();
	void test_number();
	void test_string();
	void test_array_empty();
	void test_array_numbers();
	void test_array_nested();
	void test_object_empty();
	void test_object_simple();
	void test_object_nested();
	void test_vector();
	void test_vector_nested();

	CPPUNIT_TEST_SUITE(test);
	CPPUNIT_TEST(test_null);
	CPPUNIT_TEST(test_true);
	CPPUNIT_TEST(test_false);
	CPPUNIT_TEST(test_number);
	CPPUNIT_TEST(test_string);
	CPPUNIT_TEST(test_array_empty);
	CPPUNIT_TEST(test_array_numbers);
	CPPUNIT_TEST(test_array_nested);
	CPPUNIT_TEST(test_object_empty);
	CPPUNIT_TEST(test_object_simple);
	CPPUNIT_TEST(test_object_nested);
	CPPUNIT_TEST(test_vector);
	CPPUNIT_TEST(test_vector_nested);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(test);

test::test()
{ }

void test::setUp()
{ }

void test::tearDown()
{ }

void test::test_null()
{
	std::stringstream ss;
	ss << Json::Null();
	CPPUNIT_ASSERT_EQUAL(std::string("null"), ss.str());
}

void test::test_true()
{
	std::stringstream ss;
	ss << Json::True();
	CPPUNIT_ASSERT_EQUAL(std::string("true"), ss.str());
}

void test::test_false()
{
	std::stringstream ss;
	ss << Json::False();
	CPPUNIT_ASSERT_EQUAL(std::string("false"), ss.str());
}

void test::test_number()
{
	std::stringstream ss;
	ss << Json::Number(42);
	CPPUNIT_ASSERT_EQUAL(std::string("42"), ss.str());
	ss.str("");

	ss << Json::Number(-42);
	CPPUNIT_ASSERT_EQUAL(std::string("-42"), ss.str());
	ss.str("");

	ss << Json::Number(0.0);
	CPPUNIT_ASSERT_EQUAL(std::string("0.000000"), ss.str());
	ss.str("");

	ss << Json::Number(0.00005);
	CPPUNIT_ASSERT_EQUAL(std::string("0.000050"), ss.str());
	ss.str("");
}

void test::test_string()
{
	std::stringstream ss;
	ss << Json::String();
	CPPUNIT_ASSERT_EQUAL(std::string("\"\""), ss.str());
	ss.str("");

	ss << Json::String("Hello, World!");
	CPPUNIT_ASSERT_EQUAL(std::string("\"Hello, World!\""), ss.str());
	ss.str("");

	std::string in;
	for (int i(0); i < 0x20; ++i) {
		in.push_back(char(i));
	}
	ss << Json::String(in);

	std::string expected(
		"\""
		"\\u0000\\u0001\\u0002\\u0003\\u0004\\u0005\\u0006\\u0007"
		"\\b\\t\\n\\u000b\\f\\r\\u000e\\u000f"
		"\\u0010\\u0011\\u0012\\u0013\\u0014\\u0015\\u0016\\u0017"
		"\\u0018\\u0019\\u001a\\u001b\\u001c\\u001d\\u001e\\u001f"
		"\""
	);
	CPPUNIT_ASSERT_EQUAL(expected, ss.str());
	ss.str("");
}

void test::test_array_empty()
{
	std::stringstream ss;
	ss << Json::Array();
	CPPUNIT_ASSERT_EQUAL(std::string("[]"), ss.str());
}

void test::test_array_numbers()
{
	std::stringstream ss;

	Json::Array a;
	a << 1 << 2 << 3 << 4;
	ss << a;

	std::string expected(
		"[\n"
		"	1,\n"
		"	2,\n"
		"	3,\n"
		"	4\n"
		"]"
	);
	CPPUNIT_ASSERT_EQUAL(expected, ss.str());
}

void test::test_array_nested()
{
	std::stringstream ss;

	Json::Array a;
	Json::Array a1;
	a1 << 1 << 2;
	Json::Array a2;
	a2 << true << false;
	a << a1;
	a << a2;
	a << Json::Array();
	ss << a;

	std::string expected(
		"[\n"
		"	[\n"
		"		1,\n"
		"		2\n"
		"	],\n"
		"	[\n"
		"		true,\n"
		"		false\n"
		"	],\n"
		"	[]\n"
		"]"
	);
	CPPUNIT_ASSERT_EQUAL(expected, ss.str());
}

void test::test_object_empty()
{
	std::stringstream ss;
	ss << Json::Object();
	CPPUNIT_ASSERT_EQUAL(std::string("{}"), ss.str());
}

void test::test_object_simple()
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

void test::test_object_nested()
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

void test::test_vector()
{
	std::stringstream ss;
	std::vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);
	ss << v;

	std::string expected(
		"[\n"
		"	1,\n"
		"	2,\n"
		"	3,\n"
		"	4\n"
		"]"
	);
	CPPUNIT_ASSERT_EQUAL(expected, ss.str());
}

void test::test_vector_nested()
{
	std::stringstream ss;
	std::vector<std::vector<int> > v;
	std::vector<int> v1;
	v1.push_back(1);
	v1.push_back(2);
	v.push_back(v1);
	v.push_back(v1);
	v.push_back(std::vector<int>());
	ss << v;

	std::string expected(
		"[\n"
		"	[\n"
		"		1,\n"
		"		2\n"
		"	],\n"
		"	[\n"
		"		1,\n"
		"		2\n"
		"	],\n"
		"	[]\n"
		"]"
	);
	CPPUNIT_ASSERT_EQUAL(expected, ss.str());
}

}}
