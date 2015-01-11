#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include <jsoncc.h>

std::ostream & operator<<(std::ostream & os, Json::TaggedType::Tag tag)
{
	switch (tag) {
	case Json::TaggedType::TAG_INVALID: os << "INVALID"; break;
	case Json::TaggedType::TAG_NULL:    os << "NULL";    break;
	case Json::TaggedType::TAG_TRUE:    os << "TRUE";    break;
	case Json::TaggedType::TAG_FALSE:   os << "FALSE";   break;
	case Json::TaggedType::TAG_NUMBER:  os << "NUMBER";  break;
	case Json::TaggedType::TAG_STRING:  os << "STRING";  break;
	case Json::TaggedType::TAG_OBJECT:  os << "OBJECT";  break;
	case Json::TaggedType::TAG_ARRAY:   os << "ARRAY";   break;
	}

	return os;
}

namespace unittests {
namespace tagged_type {

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
	Json::TaggedType t1;
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_INVALID, t1.tag());

	t1 = Json::TaggedType();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_INVALID, t1.tag());

	t1.clear();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_INVALID, t1.tag());

	Json::TaggedType t2(t1);
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_INVALID, t2.tag());

	Json::TaggedType t3;
	t3 = t1;
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_INVALID, t3.tag());

	Json::TaggedType t4;
	t4.clone(t1);
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_INVALID, t4.tag());

	Json::TaggedType t5;
	t5.move(t1);
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_INVALID, t1.tag());
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_INVALID, t5.tag());
}

void test::test_null_type()
{
	Json::TaggedType t1;
	t1 = Json::Null();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_NULL, t1.tag());

	t1 = Json::TaggedType();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_INVALID, t1.tag());

	t1 = Json::Null();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_NULL, t1.tag());
	t1.clear();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_INVALID, t1.tag());

	t1 = Json::Null();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_NULL, t1.tag());
	Json::TaggedType t2(t1);
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_NULL, t2.tag());

	Json::TaggedType t3;
	t3 = t1;
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_NULL, t3.tag());

	Json::TaggedType t4;
	t4.clone(t1);
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_NULL, t4.tag());

	Json::TaggedType t5;
	t5.move(t1);
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_INVALID, t1.tag());
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_NULL, t5.tag());

	Json::Null value(t5.null());
	std::stringstream ss;
	ss << value;
	CPPUNIT_ASSERT_EQUAL(std::string("null"), ss.str());
}

void test::test_true_type()
{
	Json::TaggedType t1;
	t1 = Json::True();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_TRUE, t1.tag());

	t1 = Json::TaggedType();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_INVALID, t1.tag());

	t1 = Json::True();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_TRUE, t1.tag());
	t1.clear();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_INVALID, t1.tag());

	t1 = Json::True();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_TRUE, t1.tag());
	Json::TaggedType t2(t1);
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_TRUE, t2.tag());

	Json::TaggedType t3;
	t3 = t1;
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_TRUE, t3.tag());

	Json::TaggedType t4;
	t4.clone(t1);
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_TRUE, t4.tag());

	Json::TaggedType t5;
	t5.move(t1);
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_INVALID, t1.tag());
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_TRUE, t5.tag());

	Json::True value(t5.true_value());
	std::stringstream ss;
	ss << value;
	CPPUNIT_ASSERT_EQUAL(std::string("true"), ss.str());
}

void test::test_false_type()
{
	Json::TaggedType t1;
	t1 = Json::False();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_FALSE, t1.tag());

	t1 = Json::TaggedType();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_INVALID, t1.tag());

	t1 = Json::False();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_FALSE, t1.tag());
	t1.clear();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_INVALID, t1.tag());

	t1 = Json::False();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_FALSE, t1.tag());
	Json::TaggedType t2(t1);
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_FALSE, t2.tag());

	Json::TaggedType t3;
	t3 = t1;
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_FALSE, t3.tag());

	Json::TaggedType t4;
	t4.clone(t1);
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_FALSE, t4.tag());

	Json::TaggedType t5;
	t5.move(t1);
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_INVALID, t1.tag());
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_FALSE, t5.tag());

	Json::False value(t5.false_value());
	std::stringstream ss;
	ss << value;
	CPPUNIT_ASSERT_EQUAL(std::string("false"), ss.str());
}

void test::test_number_type()
{
	Json::TaggedType t1;
	t1 = Json::Number();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_NUMBER, t1.tag());

	t1 = Json::TaggedType();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_INVALID, t1.tag());

	t1 = Json::Number(5);
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_NUMBER, t1.tag());
	t1.clear();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_INVALID, t1.tag());

	t1 = Json::Number(5);
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_NUMBER, t1.tag());
	Json::TaggedType t2(t1);
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_NUMBER, t2.tag());

	Json::TaggedType t3;
	t3 = t1;
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_NUMBER, t3.tag());

	Json::TaggedType t4;
	t4.clone(t1);
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_NUMBER, t4.tag());

	Json::TaggedType t5;
	t5.move(t1);
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_INVALID, t1.tag());
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_NUMBER, t5.tag());

	Json::Number value(t5.number());
	std::stringstream ss;
	ss << value;
	CPPUNIT_ASSERT_EQUAL(std::string("5"), ss.str());
}

void test::test_string_type()
{
	Json::TaggedType t1;
	t1 = Json::String();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_STRING, t1.tag());

	t1 = Json::TaggedType();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_INVALID, t1.tag());

	t1 = Json::String("foo");
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_STRING, t1.tag());
	t1.clear();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_INVALID, t1.tag());

	t1 = Json::String("foo");
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_STRING, t1.tag());
	Json::TaggedType t2(t1);
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_STRING, t2.tag());

	Json::TaggedType t3;
	t3 = t1;
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_STRING, t3.tag());

	Json::TaggedType t4;
	t4.clone(t1);
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_STRING, t4.tag());

	Json::TaggedType t5;
	t5.move(t1);
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_INVALID, t1.tag());
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_STRING, t5.tag());

	Json::String value(t5.string());
	std::stringstream ss;
	ss << value;
	CPPUNIT_ASSERT_EQUAL(std::string("\"foo\""), ss.str());
}

void test::test_object_type()
{
	Json::TaggedType t1;
	t1 = Json::Object();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_OBJECT, t1.tag());

	t1 = Json::TaggedType();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_INVALID, t1.tag());

	t1 = Json::Object();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_OBJECT, t1.tag());
	t1.clear();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_INVALID, t1.tag());

	t1 = Json::Object();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_OBJECT, t1.tag());
	Json::TaggedType t2(t1);
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_OBJECT, t2.tag());

	Json::TaggedType t3;
	t3 = t1;
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_OBJECT, t3.tag());

	Json::TaggedType t4;
	t4.clone(t1);
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_OBJECT, t4.tag());

	Json::TaggedType t5;
	t5.move(t1);
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_INVALID, t1.tag());
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_OBJECT, t5.tag());

	Json::Object value(t5.object());
	std::stringstream ss;
	ss << value;
	CPPUNIT_ASSERT_EQUAL(std::string("{}"), ss.str());
}

void test::test_array_type()
{
	Json::TaggedType t1;
	t1 = Json::Array();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_ARRAY, t1.tag());

	t1 = Json::TaggedType();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_INVALID, t1.tag());

	t1 = Json::Array();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_ARRAY, t1.tag());
	t1.clear();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_INVALID, t1.tag());

	t1 = Json::Array();
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_ARRAY, t1.tag());
	Json::TaggedType t2(t1);
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_ARRAY, t2.tag());

	Json::TaggedType t3;
	t3 = t1;
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_ARRAY, t3.tag());

	Json::TaggedType t4;
	t4.clone(t1);
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_ARRAY, t4.tag());

	Json::TaggedType t5;
	t5.move(t1);
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_INVALID, t1.tag());
	CPPUNIT_ASSERT_EQUAL(Json::TaggedType::TAG_ARRAY, t5.tag());

	Json::Array value(t5.array());
	std::stringstream ss;
	ss << value;
	CPPUNIT_ASSERT_EQUAL(std::string("[]"), ss.str());
}

}}
