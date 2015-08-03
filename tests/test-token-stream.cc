#include <cppunit/extensions/HelperMacros.h>

#include <math.h>

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
	void test_int_invalid_zero();
	void test_int_leading_zero();
	void test_number_delimiter();
	void test_float_zero();
	void test_float_neg_zero();
	void test_float_zeros();
	void test_float_number();
	void test_float_missing_frac();
	void test_float_missing_int();
	void test_float_missing_exp();
	void test_float_missing_plus_exp();
	void test_float_missing_minus_exp();
	void test_float_exp_zero();
	void test_float_exp_neg();
	void test_float_exp_pos();
	void test_float_exp_plus();
	void test_float_double_dot();

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
	CPPUNIT_TEST(test_int_invalid_zero);
	CPPUNIT_TEST(test_int_leading_zero);
	CPPUNIT_TEST(test_float_zero);
	CPPUNIT_TEST(test_float_neg_zero);
	CPPUNIT_TEST(test_float_zeros);
	CPPUNIT_TEST(test_float_number);
	CPPUNIT_TEST(test_float_missing_frac);
	CPPUNIT_TEST(test_float_missing_int);
	CPPUNIT_TEST(test_float_missing_exp);
	CPPUNIT_TEST(test_float_missing_plus_exp);
	CPPUNIT_TEST(test_float_missing_minus_exp);
	CPPUNIT_TEST(test_float_exp_zero);
	CPPUNIT_TEST(test_float_exp_neg);
	CPPUNIT_TEST(test_float_exp_pos);
	CPPUNIT_TEST(test_float_exp_plus);
	CPPUNIT_TEST(test_float_double_dot);
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

void test::test_int_invalid_zero()
{
	char data[] = "00";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::NUMBER, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(Token::INT, ts.token.number_type);
	CPPUNIT_ASSERT_EQUAL(int64_t(0), ts.token.int_value);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::NUMBER, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(Token::INT, ts.token.number_type);
	CPPUNIT_ASSERT_EQUAL(int64_t(0), ts.token.int_value);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Utf8Stream::SEOF, us.state());
}

void test::test_int_leading_zero()
{
	char data[] = "01234";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::NUMBER, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(Token::INT, ts.token.number_type);
	CPPUNIT_ASSERT_EQUAL(int64_t(0), ts.token.int_value);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::NUMBER, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(Token::INT, ts.token.number_type);
	CPPUNIT_ASSERT_EQUAL(int64_t(1234), ts.token.int_value);
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

void test::test_float_zero()
{
	char data[] = "0.0";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::NUMBER, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(Token::FLOAT, ts.token.number_type);
	CPPUNIT_ASSERT_EQUAL((long double)(0), ts.token.float_value);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Utf8Stream::SEOF, us.state());
}

void test::test_float_zeros()
{
	char data[] = "0.0000000";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::NUMBER, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(Token::FLOAT, ts.token.number_type);
	CPPUNIT_ASSERT_EQUAL((long double)(0), ts.token.float_value);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Utf8Stream::SEOF, us.state());
}

void test::test_float_neg_zero()
{
	char data[] = "-0.0";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::NUMBER, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(Token::FLOAT, ts.token.number_type);
	CPPUNIT_ASSERT_EQUAL((long double)(0), ts.token.float_value);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Utf8Stream::SEOF, us.state());
}

void test::test_float_number()
{
	char data[] = "1.000005";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::NUMBER, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(Token::FLOAT, ts.token.number_type);
	long double delta(fabsl((long double)(1.000005) - ts.token.float_value));
	CPPUNIT_ASSERT((long double)(1.0E-10) > delta);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Utf8Stream::SEOF, us.state());
}

void test::test_float_exp_zero()
{
	char data[] = "1.5E0";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::NUMBER, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(Token::FLOAT, ts.token.number_type);
	long double delta(fabsl((long double)(1.5) - ts.token.float_value));
	CPPUNIT_ASSERT((long double)(1.0E-10) > delta);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Utf8Stream::SEOF, us.state());
}

void test::test_float_exp_neg()
{
	char data[] = "1.5E-2";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::NUMBER, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(Token::FLOAT, ts.token.number_type);
	long double delta(fabsl((long double)(1.5E-2) - ts.token.float_value));
	CPPUNIT_ASSERT((long double)(1.0E-10) > delta);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Utf8Stream::SEOF, us.state());
}

void test::test_float_exp_pos()
{
	char data[] = "1.5E2";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::NUMBER, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(Token::FLOAT, ts.token.number_type);
	CPPUNIT_ASSERT_EQUAL((long double)(1.5E2), ts.token.float_value);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Utf8Stream::SEOF, us.state());
}

void test::test_float_exp_plus()
{
	char data[] = "1.5E+2";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::NUMBER, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(Token::FLOAT, ts.token.number_type);
	CPPUNIT_ASSERT_EQUAL((long double)(1.5E2), ts.token.float_value);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Utf8Stream::SEOF, us.state());
}

void test::test_float_missing_frac()
{
	char data[] = "1.";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(Token::NONE, ts.token.number_type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Utf8Stream::SEOF, us.state());
}

void test::test_float_missing_int()
{
	char data[] = "-.0";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(Token::NONE, ts.token.number_type);
}

void test::test_float_double_dot()
{
	char data[] = "1..4";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(Token::NONE, ts.token.number_type);
}

void test::test_float_missing_exp()
{
	char data[] = "1.0E";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(Token::NONE, ts.token.number_type);
}

void test::test_float_missing_plus_exp()
{
	char data[] = "1.0E+";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(Token::NONE, ts.token.number_type);
}

void test::test_float_missing_minus_exp()
{
	char data[] = "1.0E-";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(Token::NONE, ts.token.number_type);
}

}}
