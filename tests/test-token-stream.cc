#include <math.h>
#include <string.h>

#include "error-assert.h"
#include "error-io.h"
#include "token-stream.h"
#include "utf8stream.h"

namespace Json {

// LCOV_EXCL_START
std::ostream & operator<<(std::ostream & os, Token::Type type)
{
#define CASE_TOKEN_TYPE(name) case name: os << # name; break
	switch (type) {
	CASE_TOKEN_TYPE(Token::END);
	CASE_TOKEN_TYPE(Token::INVALID);
	CASE_TOKEN_TYPE(Token::BEGIN_ARRAY);
	CASE_TOKEN_TYPE(Token::END_ARRAY);
	CASE_TOKEN_TYPE(Token::BEGIN_OBJECT);
	CASE_TOKEN_TYPE(Token::END_OBJECT);
	CASE_TOKEN_TYPE(Token::NAME_SEPARATOR);
	CASE_TOKEN_TYPE(Token::VALUE_SEPARATOR);
	CASE_TOKEN_TYPE(Token::TRUE_LITERAL);
	CASE_TOKEN_TYPE(Token::FALSE_LITERAL);
	CASE_TOKEN_TYPE(Token::NULL_LITERAL);
	CASE_TOKEN_TYPE(Token::STRING);
	CASE_TOKEN_TYPE(Token::NUMBER);
	}
#undef CASE_TOKEN_TYPE
	return os;
}
// LCOV_EXCL_STOP

}

namespace unittests {
namespace token_stream {

class test : public CppUnit::TestCase {
public:
	test();
	void setUp();
	void tearDown();

private:
	void test_stream_zero();
	void test_bad_utf8();
	void test_utf8_bom();
	void test_structural();
	void test_invalid_token();
	void test_literal();
	void test_invalid_literal();
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
	void test_int_overflow();
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
	void test_float_overflow();
	void test_empty_string();
	void test_unterminated_string();
	void test_ascii_string();
	void test_control_string();
	void test_unicode_string();
	void test_ctrl_esc_string();
	void test_utf8_esc_string();
	void test_zero_esc_string();
	void test_surrogate_string();
	void test_utf8_incomplete_string();
	void test_invalid_esc_string();
	void test_eof();

	CPPUNIT_TEST_SUITE(test);
	CPPUNIT_TEST(test_stream_zero);
	CPPUNIT_TEST(test_bad_utf8);
	CPPUNIT_TEST(test_utf8_bom);
	CPPUNIT_TEST(test_structural);
	CPPUNIT_TEST(test_invalid_token);
	CPPUNIT_TEST(test_literal);
	CPPUNIT_TEST(test_invalid_literal);
	CPPUNIT_TEST(test_empty_array);
	CPPUNIT_TEST(test_nospace_array);
	CPPUNIT_TEST(test_int_zero);
	CPPUNIT_TEST(test_int_neg_zero);
	CPPUNIT_TEST(test_int_digit);
	CPPUNIT_TEST(test_int_neg_digit);
	CPPUNIT_TEST(test_int_number);
	CPPUNIT_TEST(test_int_neg_number);
	CPPUNIT_TEST(test_int_overflow);
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
	CPPUNIT_TEST(test_float_overflow);
	CPPUNIT_TEST(test_empty_string);
	CPPUNIT_TEST(test_unterminated_string);
	CPPUNIT_TEST(test_ascii_string);
	CPPUNIT_TEST(test_control_string);
	CPPUNIT_TEST(test_unicode_string);
	CPPUNIT_TEST(test_ctrl_esc_string);
	CPPUNIT_TEST(test_utf8_esc_string);
	CPPUNIT_TEST(test_zero_esc_string);
	CPPUNIT_TEST(test_surrogate_string);
	CPPUNIT_TEST(test_utf8_incomplete_string);
	CPPUNIT_TEST(test_invalid_esc_string);
	CPPUNIT_TEST(test_eof);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(test);

test::test()
{ }

void test::setUp()
{ }

void test::tearDown()
{ }

using namespace Json;

void test::test_stream_zero()
{
	char data[] = "  \0  {}";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(ts.scan(), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::STREAM_ZERO, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(2), error.location.offs);
}

void test::test_bad_utf8()
{
	char data[] = "  \xa9 {}";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(ts.scan(), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::UTF8_INVALID, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(2), error.location.offs);
}

void test::test_utf8_bom()
{
	char data[] = "\xef\xbb\xbf {}";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(ts.scan(), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::TOKEN_INVALID, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(1), error.location.offs);
}

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
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
}

void test::test_invalid_token()
{
	char data[] = "hello";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(ts.scan(), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::TOKEN_INVALID, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(1), error.location.offs);
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
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
}

void test::test_invalid_literal()
{
	char data[] = "truE";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(ts.scan(), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::LITERAL_INVALID, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(4), error.location.offs);
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
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
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
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
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
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
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
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
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
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
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
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
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
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
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
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
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
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
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
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
}

void test::test_int_overflow()
{
	char data[2048];
	memset(data, '0', sizeof(data) - 1);
	data[0] = '1';

	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(ts.scan(), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::NUMBER_OVERFLOW, error.type);
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
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
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
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
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
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
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
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
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
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
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
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
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
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
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
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
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
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
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
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
}

void test::test_float_missing_frac()
{
	char data[] = "1.";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(ts.scan(), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::NUMBER_INVALID, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(2), error.location.offs);
}

void test::test_float_missing_int()
{
	char data[] = "-.0";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(ts.scan(), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::NUMBER_INVALID, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(2), error.location.offs);
}

void test::test_float_double_dot()
{
	char data[] = "1..4";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(ts.scan(), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::NUMBER_INVALID, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(3), error.location.offs);
}

void test::test_float_missing_exp()
{
	char data[] = "1.0E";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(ts.scan(), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::NUMBER_INVALID, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(4), error.location.offs);
}

void test::test_float_missing_plus_exp()
{
	char data[] = "1.0E+";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(ts.scan(), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::NUMBER_INVALID, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(5), error.location.offs);
}

void test::test_float_missing_minus_exp()
{
	char data[] = "1.0E-";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(ts.scan(), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::NUMBER_INVALID, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(5), error.location.offs);
}

void test::test_float_overflow()
{
	char data[2048];
	memset(data, '0', sizeof(data) - 1);
	data[0] = '1';
	data[1] = '.';

	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(ts.scan(), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::NUMBER_OVERFLOW, error.type);
}

void test::test_empty_string()
{
	char data[] = "\"\"";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::STRING, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(std::string(), ts.token.str_value);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
}

void test::test_unterminated_string()
{
	char data[] = "\"";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(ts.scan(), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::STRING_QUOTE, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(1), error.location.offs);
}

void test::test_ascii_string()
{
	char data[] = "\"Hello, World!\"";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::STRING, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(std::string("Hello, World!"), ts.token.str_value);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
}

void test::test_control_string()
{
	char data[] = "\"Hello, World!\n\"";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(ts.scan(), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::STRING_CTRL, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(15), error.location.offs);
}

void test::test_unicode_string()
{
	char data[] = "\"ἀνερρίφθω κύβος\"";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::STRING, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(std::string("ἀνερρίφθω κύβος"), ts.token.str_value);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
}

void test::test_ctrl_esc_string()
{
	char data[] = "\"\\\\\\/\\\"\\b\\f\\n\\r\\t\"";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::STRING, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(std::string("\\/\"\b\f\n\r\t"), ts.token.str_value);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
}

void test::test_utf8_esc_string()
{
	// use code points which encode into one, two and three byte
	// utf8 sequences.
	char data[] = "\"\\u0065\\u00DC\\u1d2800\"";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::STRING, ts.token.type);
	CPPUNIT_ASSERT_EQUAL(std::string("eÜᴨ00"), ts.token.str_value);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
}

void test::test_surrogate_string()
{
	// utf16 surrogate
	char data[] = "\"\\udafe\"";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(ts.scan(), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::UESCAPE_SURROGATE, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(7), error.location.offs);
}

void test::test_zero_esc_string()
{
	char data[] = "\"Hello,\\u0000\"";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(ts.scan(), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::UESCAPE_ZERO, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(13), error.location.offs);
}

void test::test_utf8_incomplete_string()
{
	char data[] = "\"\\u00AX\"";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(ts.scan(), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::UESCAPE_INVALID, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(7), error.location.offs);
}

void test::test_invalid_esc_string()
{
	// utf16 surrogate
	char data[] = "\"\\x\"";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(ts.scan(), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::ESCAPE_INVALID, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(3), error.location.offs);
}

void test::test_eof()
{
	char data[] = "";
	Utf8Stream us(data, sizeof(data) - 1);
	TokenStream ts(us);

	CPPUNIT_ASSERT_EQUAL(Token::INVALID, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
	ts.scan();
	CPPUNIT_ASSERT_EQUAL(Token::END, ts.token.type);
}

}}
