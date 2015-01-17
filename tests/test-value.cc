#include <cppunit/extensions/HelperMacros.h>
#include <jsoncc.h>

std::ostream & operator<<(std::ostream & os, Json::Value::Tag tag)
{
	switch (tag) {
	case Json::Value::TAG_INVALID: os << "INVALID"; break;
	case Json::Value::TAG_NULL:    os << "NULL";    break;
	case Json::Value::TAG_TRUE:    os << "TRUE";    break;
	case Json::Value::TAG_FALSE:   os << "FALSE";   break;
	case Json::Value::TAG_NUMBER:  os << "NUMBER";  break;
	case Json::Value::TAG_STRING:  os << "STRING";  break;
	case Json::Value::TAG_OBJECT:  os << "OBJECT";  break;
	case Json::Value::TAG_ARRAY:   os << "ARRAY";   break;
	}

	return os;
}

namespace unittests {
namespace value {

class test : public CppUnit::TestCase {
public:
	test();
	void setUp();
	void tearDown();

private:
	void test_invalid_type();
	void test_null_type();
	void test_true_type();
	void test_false_type();
	void test_number_type();
	void test_string_type();
	void test_object_type();
	void test_array_type();

	CPPUNIT_TEST_SUITE(test);
	CPPUNIT_TEST(test_invalid_type);
	CPPUNIT_TEST(test_null_type);
	CPPUNIT_TEST(test_true_type);
	CPPUNIT_TEST(test_false_type);
	CPPUNIT_TEST(test_number_type);
	CPPUNIT_TEST(test_string_type);
	CPPUNIT_TEST(test_object_type);
	CPPUNIT_TEST(test_array_type);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(test);

test::test()
{ }

void test::setUp()
{ }

void test::tearDown()
{ }

void test::test_invalid_type()
{
	Json::Value t1;
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_INVALID, t1.tag());

	t1 = Json::Value();
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_INVALID, t1.tag());

	Json::Value t2(t1);
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_INVALID, t2.tag());

	Json::Value t3;
	t3 = t1;
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_INVALID, t3.tag());

	CPPUNIT_ASSERT_EQUAL(t1, t3);
}

void test::test_null_type()
{
	Json::Value t1;
	t1.set(Json::Null());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NULL, t1.tag());

	t1 = Json::Value();
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_INVALID, t1.tag());

	t1.set(Json::Null());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NULL, t1.tag());

	Json::Value t2(t1);
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NULL, t2.tag());

	Json::Value t3;
	t3 = t1;
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NULL, t3.tag());

	CPPUNIT_ASSERT_EQUAL(t1, t3);
}

void test::test_true_type()
{
	Json::Value t1;
	t1.set(Json::True());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_TRUE, t1.tag());

	t1 = Json::Value();
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_INVALID, t1.tag());

	t1.set(Json::True());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_TRUE, t1.tag());

	Json::Value t2(t1);
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_TRUE, t2.tag());

	Json::Value t3;
	t3 = t1;
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_TRUE, t3.tag());

	CPPUNIT_ASSERT_EQUAL(t1, t3);
}

void test::test_false_type()
{
	Json::Value t1;
	t1.set(Json::False());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_FALSE, t1.tag());

	t1 = Json::Value();
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_INVALID, t1.tag());

	t1.set(Json::False());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_FALSE, t1.tag());

	Json::Value t2(t1);
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_FALSE, t2.tag());

	Json::Value t3;
	t3 = t1;
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_FALSE, t3.tag());

	CPPUNIT_ASSERT_EQUAL(t1, t3);
}

void test::test_number_type()
{
	Json::Value t1;
	t1.set(Json::Number(5));
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, t1.tag());

	t1 = Json::Value();
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_INVALID, t1.tag());

	t1.set(Json::Number(5));
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, t1.tag());

	Json::Value t2(t1);
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, t2.tag());

	Json::Value t3;
	t3 = t1;
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, t3.tag());

	CPPUNIT_ASSERT_EQUAL(t1, t3);
}

void test::test_string_type()
{
	Json::Value t1;
	t1.set(Json::String("foo"));
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_STRING, t1.tag());

	t1 = Json::Value();
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_INVALID, t1.tag());

	t1.set(Json::String("foo"));
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_STRING, t1.tag());

	Json::Value t2(t1);
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_STRING, t2.tag());

	Json::Value t3;
	t3 = t1;
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_STRING, t3.tag());

	CPPUNIT_ASSERT_EQUAL(t1, t3);
}

void test::test_object_type()
{
	Json::Value t1;
	t1.set(Json::Object());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_OBJECT, t1.tag());

	t1 = Json::Value();
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_INVALID, t1.tag());

	t1.set(Json::Object());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_OBJECT, t1.tag());

	Json::Value t2(t1);
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_OBJECT, t2.tag());

	Json::Value t3;
	t3 = t1;
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_OBJECT, t3.tag());

	CPPUNIT_ASSERT_EQUAL(t1, t3);
}

void test::test_array_type()
{
	Json::Value t1;
	t1.set(Json::Array());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_ARRAY, t1.tag());

	t1 = Json::Value();
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_INVALID, t1.tag());

	t1.set(Json::Array());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_ARRAY, t1.tag());

	Json::Value t2(t1);
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_ARRAY, t2.tag());

	Json::Value t3;
	t3 = t1;
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_ARRAY, t3.tag());

	CPPUNIT_ASSERT_EQUAL(t1, t3);
}

}}
