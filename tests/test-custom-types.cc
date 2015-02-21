#include <cppunit/extensions/HelperMacros.h>

namespace {

struct foo {
	int a;
	bool b;
	std::string c;
};

struct bar { };

enum Baz {
	BAZ_1,
	BAZ_2,
	BAZ_3,
};

std::ostream & operator<<(std::ostream & os, ::foo const&)
{
	return os << "Foo Object";
}

std::ostream & operator<<(std::ostream & os, ::Baz baz)
{
	switch (baz) {
	case BAZ_1: os << "BAZ_1"; break;
	case BAZ_2: os << "BAZ_2"; break;
	case BAZ_3: os << "BAZ_3"; break;
	}

	return os;
}

}

// we have to place it here to keep clang++ happy.
// It will warn about unused std::ostream & operator<<(...)
// for the above otherwise...
#include <jsoncc.h>

namespace Json {

template<> struct ValueFactory< ::foo> {
	static void build(foo const& f, Value & res)
	{
		Json::Object o;
		o << Json::Member("a", Json::Number(f.a));
		o << Json::Member("b", f.b);
		o << Json::Member("c", f.c);
		res.set(o);
	}
};

}

namespace unittests {
namespace custom_type {

class test : public CppUnit::TestCase {
public:
	test();
	void setUp();
	void tearDown();

private:
	void test_custom_type();
	void test_custom_type_vector();
	void test_streamable_object();
	void test_streamable_enum();

	CPPUNIT_TEST_SUITE(test);
	CPPUNIT_TEST(test_streamable_object);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(test);

test::test()
{ }

void test::setUp()
{ }

void test::tearDown()
{ }

void test::test_custom_type()
{
	struct foo f;
	f.a = 42;
	f.b = false;
	f.c = "Foo";

	std::stringstream ss;
	ss << f;
	CPPUNIT_ASSERT_EQUAL(std::string("Foo Object"), ss.str());
	ss.str("");

	ss << Json::Value(f);
	std::string expected(
		"{\n"
		"	\"a\": 42,\n"
		"	\"b\": false,\n"
		"	\"c\": \"Foo\"\n"
		"}"
	);

	CPPUNIT_ASSERT_EQUAL(expected, ss.str());
}

void test::test_custom_type_vector()
{
	foo f;
	f.a = 42;
	f.b = false;
	f.c = "Foo";

	std::vector<foo> v;
	v.push_back(f);
	v.push_back(f);

	std::stringstream ss;
	ss << Json::Value(v);

	std::string expected(
		"[\n"
		"	{\n"
		"		\"a\": 42,\n"
		"		\"b\": false,\n"
		"		\"c\": \"Foo\"\n"
		"	},\n"
		"	{\n"
		"		\"a\": 42,\n"
		"		\"b\": false,\n"
		"		\"c\": \"Foo\"\n"
		"	}\n"
		"]"
	);

	CPPUNIT_ASSERT_EQUAL(expected, ss.str());
}

void test::test_streamable_object()
{
	::bar b;

	std::stringstream ss;
	ss << Json::Value(b); // xxx 0

	std::string expected("\"Bar Object\"");
	CPPUNIT_ASSERT_EQUAL(expected, ss.str());
}

void test::test_streamable_enum()
{
	Json::Array a;
	a << ::BAZ_1;
	a << ::BAZ_2;
	a << ::BAZ_3;

	std::stringstream ss;
	ss << a;

	std::string expected(
		"[\n"
		"	\"BAZ_1\",\n"
		"	\"BAZ_2\",\n"
		"	\"BAZ_3\"\n"
		"]"
	);
	CPPUNIT_ASSERT_EQUAL(expected, ss.str());
}

}}
