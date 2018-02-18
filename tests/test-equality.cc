#include <cppunit/extensions/HelperMacros.h>

#include <jsoncc.h>

namespace unittests {
namespace eqality {

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
	void test_array();
	void test_object();
	void test_value();

	CPPUNIT_TEST_SUITE(test);
	CPPUNIT_TEST(test_null);
	CPPUNIT_TEST(test_true);
	CPPUNIT_TEST(test_false);
	CPPUNIT_TEST(test_number);
	CPPUNIT_TEST(test_string);
	CPPUNIT_TEST(test_array);
	CPPUNIT_TEST(test_object);
	CPPUNIT_TEST(test_value);
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
	CPPUNIT_ASSERT(Json::equal(Json::Null(), Json::Null()));
}

void test::test_true()
{
	CPPUNIT_ASSERT(Json::equal(Json::True(), Json::True()));
}

void test::test_false()
{
	CPPUNIT_ASSERT(Json::equal(Json::False(), Json::False()));
}

void test::test_number()
{
	CPPUNIT_ASSERT(Json::equal(Json::Number(), Json::Number()));
	CPPUNIT_ASSERT(!Json::equal(Json::Number(int32_t(0)), Json::Number()));
	CPPUNIT_ASSERT(Json::equal(Json::Number(int32_t(1)), Json::Number(int32_t(1))));
	CPPUNIT_ASSERT(!Json::equal(Json::Number(int32_t(-1)), Json::Number(int32_t(1))));
	CPPUNIT_ASSERT(!Json::equal(Json::Number(int32_t(1)), Json::Number(uint32_t(1))));
	CPPUNIT_ASSERT(!Json::equal(Json::Number(int32_t(1)), Json::Number(double(1))));
}

void test::test_string()
{
	CPPUNIT_ASSERT(Json::equal(Json::String(), Json::String()));
	CPPUNIT_ASSERT(Json::equal(Json::String(), Json::String("")));
	CPPUNIT_ASSERT(Json::equal(Json::String(""), Json::String("")));
	CPPUNIT_ASSERT(Json::equal(Json::String("foo"), Json::String("foo")));
	CPPUNIT_ASSERT(!Json::equal(Json::String("foo"), Json::String("bar")));
	CPPUNIT_ASSERT(!Json::equal(Json::String("foo"), Json::String("Foo")));
	CPPUNIT_ASSERT(!Json::equal(Json::String("f\noo"), Json::String("f\\noo")));
}

void test::test_array()
{
	CPPUNIT_ASSERT(Json::equal(Json::Array(), Json::Array()));
	CPPUNIT_ASSERT(!Json::equal(Json::Array(), Json::Array{"foo"}));
	CPPUNIT_ASSERT(Json::equal(Json::Array{"foo"}, Json::Array{"foo"}));
	CPPUNIT_ASSERT(!Json::equal(Json::Array{"Foo"}, Json::Array{"foo"}));
	CPPUNIT_ASSERT(Json::equal(Json::Array{"foo", "bar"}, Json::Array{"foo", "bar"}));
	CPPUNIT_ASSERT(!Json::equal(Json::Array{"foo", "bar"}, Json::Array{"bar", "foo"}));
	CPPUNIT_ASSERT(!Json::equal(Json::Array{"foo"}, Json::Array{"foo", "foo"}));
}

void test::test_object()
{
	CPPUNIT_ASSERT(Json::equal(Json::Object(), Json::Object()));
	CPPUNIT_ASSERT(!Json::equal(Json::Object(), Json::Object{{}}));
	CPPUNIT_ASSERT(!Json::equal(Json::Object(), Json::Object{{"foo", 1}}));
	CPPUNIT_ASSERT(!Json::equal(Json::Object{{"foo", 2}}, Json::Object{{"foo", 1}}));
	CPPUNIT_ASSERT(!Json::equal(Json::Object{{"foo", "bar"}}, Json::Object{{"foo", 1}}));

	auto o1 = Json::Object{
		{"foo", 1},
		{"bar", 1}
	};
	auto o2 = Json::Object{
		{"bar", 1},
		{"foo", 1}
	};
	CPPUNIT_ASSERT(Json::equal(o1, o2));

	auto o3 = Json::Object{
		{"foo", 1},
		{"foo", 1}
	};
	auto o4 = Json::Object{
		{"foo", 1},
		{"foo", 1}
	};
	CPPUNIT_ASSERT(Json::equal(o3, o4));

	auto o5 = Json::Object{
		{"foo", 1},
		{"foo", 1}
	};
	auto o6 = Json::Object{
		{"foo", 1},
		{"foo", 2}
	};
	CPPUNIT_ASSERT(!Json::equal(o5, o6));

	auto o7 = Json::Object{
		{"foo", 1},
		{"foo", 1}
	};
	auto o8 = Json::Object{
		{"foo", 1}
	};
	CPPUNIT_ASSERT(!Json::equal(o7, o8));
}

void test::test_value()
{
	CPPUNIT_ASSERT(Json::equal(Json::Value(), Json::Value()));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(), Json::Value(Json::Null())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(), Json::Value(Json::True())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(), Json::Value(Json::False())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(), Json::Value(Json::Number())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(), Json::Value(Json::String())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(), Json::Value(Json::Array())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(), Json::Value(Json::Object())));

	CPPUNIT_ASSERT(Json::equal(Json::Value(Json::Null()), Json::Value(Json::Null())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::Null()), Json::Value(Json::True())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::Null()), Json::Value(Json::False())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::Null()), Json::Value(Json::Number())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::Null()), Json::Value(Json::String())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::Null()), Json::Value(Json::Array())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::Null()), Json::Value(Json::Object())));

	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::True()), Json::Value(Json::Null())));
	CPPUNIT_ASSERT(Json::equal(Json::Value(Json::True()), Json::Value(Json::True())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::True()), Json::Value(Json::False())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::True()), Json::Value(Json::Number())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::True()), Json::Value(Json::String())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::True()), Json::Value(Json::Array())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::True()), Json::Value(Json::Object())));

	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::False()), Json::Value(Json::Null())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::False()), Json::Value(Json::True())));
	CPPUNIT_ASSERT(Json::equal(Json::Value(Json::False()), Json::Value(Json::False())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::False()), Json::Value(Json::Number())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::False()), Json::Value(Json::String())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::False()), Json::Value(Json::Array())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::False()), Json::Value(Json::Object())));

	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::Number()), Json::Value(Json::Null())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::Number()), Json::Value(Json::True())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::Number()), Json::Value(Json::False())));
	CPPUNIT_ASSERT(Json::equal(Json::Value(Json::Number()), Json::Value(Json::Number())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::Number()), Json::Value(Json::String())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::Number()), Json::Value(Json::Array())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::Number()), Json::Value(Json::Object())));

	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::String()), Json::Value(Json::Null())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::String()), Json::Value(Json::True())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::String()), Json::Value(Json::False())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::String()), Json::Value(Json::Number())));
	CPPUNIT_ASSERT(Json::equal(Json::Value(Json::String()), Json::Value(Json::String())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::String()), Json::Value(Json::Array())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::String()), Json::Value(Json::Object())));

	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::Array()), Json::Value(Json::Null())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::Array()), Json::Value(Json::True())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::Array()), Json::Value(Json::False())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::Array()), Json::Value(Json::Number())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::Array()), Json::Value(Json::String())));
	CPPUNIT_ASSERT(Json::equal(Json::Value(Json::Array()), Json::Value(Json::Array())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::Array()), Json::Value(Json::Object())));

	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::Object()), Json::Value(Json::Null())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::Object()), Json::Value(Json::True())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::Object()), Json::Value(Json::False())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::Object()), Json::Value(Json::Number())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::Object()), Json::Value(Json::String())));
	CPPUNIT_ASSERT(!Json::equal(Json::Value(Json::Object()), Json::Value(Json::Array())));
	CPPUNIT_ASSERT(Json::equal(Json::Value(Json::Object()), Json::Value(Json::Object())));
}

}}
