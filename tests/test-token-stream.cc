#include <cppunit/extensions/HelperMacros.h>

#include "token-stream.h"
#include "utf8stream.h"

namespace unittests {
namespace token_stream {

class test : public CppUnit::TestCase {
public:
	test();
	void setUp();
	void tearDown();

private:
	void test_structural();
	void test_literal();
	void test_empty_array();
	void test_nospace_array();
	void test_int_zero();
	void test_int_neg_zero();
	void test_int_digit();
	void test_int_neg_digit();
	void test_int_number();
	void test_int_neg_number();
	void test_int_array();
	void test_number_delimiter();

	CPPUNIT_TEST_SUITE(test);
	CPPUNIT_TEST(test_structural);
	CPPUNIT_TEST(test_literal);
	CPPUNIT_TEST(test_empty_array);
	CPPUNIT_TEST(test_nospace_array);
	CPPUNIT_TEST(test_int_zero);
	CPPUNIT_TEST(test_int_neg_zero);
	CPPUNIT_TEST(test_int_digit);
	CPPUNIT_TEST(test_int_neg_digit);
	CPPUNIT_TEST(test_int_number);
	CPPUNIT_TEST(test_int_neg_number);
	CPPUNIT_TEST(test_number_delimiter);
	CPPUNIT_TEST(test_int_array);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(test);

test::test()
{ }

void test::setUp()
{ }

void test::tearDown()
{ }

using namespace jsonp;

void test::test_structural()
{
	char data[] = " { } [ ] : , ";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::BEGIN_OBJECT, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::END_OBJECT, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::BEGIN_ARRAY, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::END_ARRAY, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::NAME_SEPARATOR, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::VALUE_SEPARATOR, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Utf8Stream::SEOF, us.state());
}

void test::test_literal()
{
	char data[] = "true false null";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::TRUE_LITERAL, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::FALSE_LITERAL, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::NULL_LITERAL, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Utf8Stream::SEOF, us.state());
}

void test::test_empty_array()
{
	char data[] = "[]";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::BEGIN_ARRAY, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::END_ARRAY, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Utf8Stream::SEOF, us.state());
}

void test::test_nospace_array()
{
	char data[] = "[true,false,null]";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::BEGIN_ARRAY, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::TRUE_LITERAL, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::VALUE_SEPARATOR, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::FALSE_LITERAL, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::VALUE_SEPARATOR, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::NULL_LITERAL, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::END_ARRAY, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Utf8Stream::SEOF, us.state());
}

void test::test_int_zero()
{
	char data[] = "0";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::NUMBER, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(Token::INT, ts.token.number_type);
	CPPUNIT_ASSERT_EQUAL(int64_t(0), ts.token.int_value);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Utf8Stream::SEOF, us.state());
}

void test::test_int_neg_zero()
{
	char data[] = "-0";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::NUMBER, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(Token::INT, ts.token.number_type);
	CPPUNIT_ASSERT_EQUAL(int64_t(0), ts.token.int_value);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Utf8Stream::SEOF, us.state());
}

void test::test_int_digit()
{
	char data[] = "5";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::NUMBER, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(Token::INT, ts.token.number_type);
	CPPUNIT_ASSERT_EQUAL(int64_t(5), ts.token.int_value);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Utf8Stream::SEOF, us.state());
}

void test::test_int_neg_digit()
{
	char data[] = "-5";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::NUMBER, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(Token::INT, ts.token.number_type);
	CPPUNIT_ASSERT_EQUAL(int64_t(-5), ts.token.int_value);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Utf8Stream::SEOF, us.state());
}

void test::test_int_number()
{
	char data[] = "102030405060708090";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::NUMBER, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(Token::INT, ts.token.number_type);
	CPPUNIT_ASSERT_EQUAL(int64_t(102030405060708090), ts.token.int_value);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Utf8Stream::SEOF, us.state());
}

void test::test_int_neg_number()
{
	char data[] = "-102030405060708090";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::NUMBER, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(Token::INT, ts.token.number_type);
	CPPUNIT_ASSERT_EQUAL(int64_t(-102030405060708090), ts.token.int_value);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Utf8Stream::SEOF, us.state());
}

void test::test_number_delimiter()
{
	char data[] = "0,";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::NUMBER, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(Token::INT, ts.token.number_type);
	CPPUNIT_ASSERT_EQUAL(int64_t(0), ts.token.int_value);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::VALUE_SEPARATOR, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Utf8Stream::SEOF, us.state());
}

void test::test_int_array()
{
	char data[] = "[0,-1,20]";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::BEGIN_ARRAY, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::NUMBER, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(Token::INT, ts.token.number_type);
	CPPUNIT_ASSERT_EQUAL(int64_t(0), ts.token.int_value);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::VALUE_SEPARATOR, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::NUMBER, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(Token::INT, ts.token.number_type);
	CPPUNIT_ASSERT_EQUAL(int64_t(-1), ts.token.int_value);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::VALUE_SEPARATOR, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::NUMBER, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(Token::INT, ts.token.number_type);
	CPPUNIT_ASSERT_EQUAL(int64_t(20), ts.token.int_value);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::END_ARRAY, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Utf8Stream::SEOF, us.state());
}

}}
