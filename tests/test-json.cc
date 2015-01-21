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
	void test_array_equality();
	void test_array_from_container();
	void test_object_empty();
	void test_object_simple();
	void test_object_nested();
	void test_object_equality();
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
	CPPUNIT_TEST(test_array_equality);
	CPPUNIT_TEST(test_array_from_container);
	CPPUNIT_TEST(test_object_empty);
	CPPUNIT_TEST(test_object_simple);
	CPPUNIT_TEST(test_object_nested);
	CPPUNIT_TEST(test_object_equality);
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
	Json::Null n;
	CPPUNIT_ASSERT_EQUAL(n, n);
	CPPUNIT_ASSERT_EQUAL(Json::Null(), Json::Null());
}

void test::test_true()
{
	std::stringstream ss;
	ss << Json::True();
	CPPUNIT_ASSERT_EQUAL(std::string("true"), ss.str());
	Json::True t;
	CPPUNIT_ASSERT_EQUAL(t, t);
	CPPUNIT_ASSERT_EQUAL(Json::True(), Json::True());
}

void test::test_false()
{
	std::stringstream ss;
	ss << Json::False();
	CPPUNIT_ASSERT_EQUAL(std::string("false"), ss.str());
	Json::False f;
	CPPUNIT_ASSERT_EQUAL(f, f);
	CPPUNIT_ASSERT_EQUAL(Json::False(), Json::False());
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

	Json::Number n(5);
	CPPUNIT_ASSERT_EQUAL(n, n);

	CPPUNIT_ASSERT_EQUAL(Json::Number(), Json::Number());
	CPPUNIT_ASSERT_EQUAL(Json::Number(uint32_t(5)), Json::Number(uint32_t(5)));
	CPPUNIT_ASSERT_EQUAL(Json::Number(int32_t(5)), Json::Number(int32_t(5)));
	CPPUNIT_ASSERT_EQUAL(Json::Number(double(5.0)), Json::Number(double(5.0)));

	CPPUNIT_ASSERT(!(Json::Number(int32_t(5)) == Json::Number(uint32_t(5))));
	CPPUNIT_ASSERT(!(Json::Number(double(5)) == Json::Number(uint32_t(5))));
	CPPUNIT_ASSERT(!(Json::Number(int32_t(5)) == Json::Number(double(5))));
	CPPUNIT_ASSERT(!(Json::Number(0) == Json::Number()));
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

	Json::String s("foo");
	CPPUNIT_ASSERT_EQUAL(s, s);
	CPPUNIT_ASSERT_EQUAL(Json::String("foo"), Json::String("foo"));
	CPPUNIT_ASSERT_EQUAL(Json::String(""), Json::String(""));
	CPPUNIT_ASSERT_EQUAL(Json::String(), Json::String());
	CPPUNIT_ASSERT(!(Json::String("Foo") == Json::String("Bar")));
	CPPUNIT_ASSERT(!(Json::String("Foo") == Json::String()));
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

void test::test_array_equality()
{
	CPPUNIT_ASSERT_EQUAL(Json::Array(), Json::Array());
	Json::Array a1;
	CPPUNIT_ASSERT_EQUAL(a1, a1);
	a1 << 1 << 2 << 3;
	Json::Array a2(a1);
	CPPUNIT_ASSERT_EQUAL(a1, a2);
	Json::Array a3;
	a3 << 1 << 2;
	CPPUNIT_ASSERT(!(a3 == a1));
	CPPUNIT_ASSERT(!(a3 == Json::Array()));
	Json::Array a4;
	a1 << 3 << 2 << 1;
	CPPUNIT_ASSERT(!(a4 == a1));
}

void test::test_array_from_container()
{
	std::vector<uint32_t> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);

	Json::Array a1(v.begin(), v.end());
	CPPUNIT_ASSERT_EQUAL(size_t(3), a1.elements().size());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, a1.elements()[0].tag());
	CPPUNIT_ASSERT_EQUAL(uint64_t(1), a1.elements()[0].number().uint_value());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, a1.elements()[1].tag());
	CPPUNIT_ASSERT_EQUAL(uint64_t(2), a1.elements()[1].number().uint_value());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, a1.elements()[2].tag());
	CPPUNIT_ASSERT_EQUAL(uint64_t(3), a1.elements()[2].number().uint_value());

	std::set<std::string> s;
	s.insert("foo");
	s.insert("bar");
	s.insert("baz");

	Json::Array a2(s.begin(), s.end());
	CPPUNIT_ASSERT_EQUAL(size_t(3), a2.elements().size());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_STRING, a2.elements()[0].tag());
	CPPUNIT_ASSERT_EQUAL(std::string("bar"), a2.elements()[0].string().value());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_STRING, a2.elements()[1].tag());
	CPPUNIT_ASSERT_EQUAL(std::string("baz"), a2.elements()[1].string().value());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_STRING, a2.elements()[2].tag());
	CPPUNIT_ASSERT_EQUAL(std::string("foo"), a2.elements()[2].string().value());

	std::list<int32_t> l;
	l.push_back(1);
	l.push_back(2);
	l.push_back(3);

	Json::Array a3(l.begin(), l.end());
	CPPUNIT_ASSERT_EQUAL(size_t(3), a3.elements().size());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, a3.elements()[0].tag());
	CPPUNIT_ASSERT_EQUAL(int64_t(1), a3.elements()[0].number().int_value());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, a3.elements()[1].tag());
	CPPUNIT_ASSERT_EQUAL(int64_t(2), a3.elements()[1].number().int_value());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, a3.elements()[2].tag());
	CPPUNIT_ASSERT_EQUAL(int64_t(3), a3.elements()[2].number().int_value());
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

void test::test_object_equality()
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

	CPPUNIT_ASSERT(!(o1 == Json::Object()));

	Json::Object o3;
	o3 << Json::Member("number", 5);
	o3 << Json::Member("bool", true);
	o3 << Json::Member("string", "Foo");
	o3 << Json::Member("array", Json::Array());
	CPPUNIT_ASSERT(!(o1 == o3));

	// CAVEAT!
	Json::Object o4;
	o3 << Json::Member("whatever", Json::Null());
	o3 << Json::Member("array", Json::Array());
	o3 << Json::Member("string", "Foo");
	o3 << Json::Member("bool", true);
	o3 << Json::Member("number", 5);
	CPPUNIT_ASSERT(!(o1 == o4));
}

void test::test_vector()
{
	std::stringstream ss;
	std::vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);
	ss << Json::Value(v);

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
	ss << Json::Value(v);

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
