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

	CPPUNIT_TEST_SUITE(test);
	CPPUNIT_TEST(test_structural);
	CPPUNIT_TEST(test_literal);
	CPPUNIT_TEST(test_empty_array);
	CPPUNIT_TEST(test_nospace_array);
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

}}
