#include <cppunit/extensions/HelperMacros.h>

#include "utf8stream.h"

namespace unittests {
namespace utf8stream {

class test : public CppUnit::TestCase {
public:
	test();
	void setUp();
	void tearDown();

private:
	void test_empty();
	void test_onebyte();
	void test_simple();

	CPPUNIT_TEST_SUITE(test);
	CPPUNIT_TEST(test_empty);
	CPPUNIT_TEST(test_onebyte);
	CPPUNIT_TEST(test_simple);
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

void test::test_empty()
{
	Utf8Stream us(NULL, 0);
	CPPUNIT_ASSERT_EQUAL(Utf8Stream::SEOF, us.state());
	CPPUNIT_ASSERT_EQUAL(int(Utf8Stream::SEOF), us.getc());
	us.ungetc();
	CPPUNIT_ASSERT_EQUAL(int(Utf8Stream::SEOF), us.getc());
}

void test::test_onebyte()
{
	char data[] = "X";
	Utf8Stream us(data, 1);
	CPPUNIT_ASSERT_EQUAL(Utf8Stream::SGOOD, us.state());
	CPPUNIT_ASSERT_EQUAL(int('X'), us.getc());
	us.ungetc();
	CPPUNIT_ASSERT_EQUAL(int('X'), us.getc());
	CPPUNIT_ASSERT_EQUAL(int(Utf8Stream::SEOF), us.getc());
	us.ungetc();
	us.ungetc();
	us.ungetc();
	CPPUNIT_ASSERT_EQUAL(int('X'), us.getc());
	CPPUNIT_ASSERT_EQUAL(int(Utf8Stream::SEOF), us.getc());
}

void test::test_simple()
{
	char data[] = "Hello World!";
	Utf8Stream us(data, sizeof(data) - 1);
	CPPUNIT_ASSERT_EQUAL(Utf8Stream::SGOOD, us.state());
	CPPUNIT_ASSERT_EQUAL(int('H'), us.getc());
	CPPUNIT_ASSERT_EQUAL(int('e'), us.getc());
	CPPUNIT_ASSERT_EQUAL(int('l'), us.getc());
	CPPUNIT_ASSERT_EQUAL(int('l'), us.getc());

	us.ungetc();
	us.ungetc();
	CPPUNIT_ASSERT_EQUAL(int('l'), us.getc());
	CPPUNIT_ASSERT_EQUAL(int('l'), us.getc());

	CPPUNIT_ASSERT_EQUAL(int('o'), us.getc());
	CPPUNIT_ASSERT_EQUAL(int(' '), us.getc());
	CPPUNIT_ASSERT_EQUAL(int('W'), us.getc());
	CPPUNIT_ASSERT_EQUAL(int('o'), us.getc());
	CPPUNIT_ASSERT_EQUAL(int('r'), us.getc());
	CPPUNIT_ASSERT_EQUAL(int('l'), us.getc());
	CPPUNIT_ASSERT_EQUAL(int('d'), us.getc());
	CPPUNIT_ASSERT_EQUAL(int('!'), us.getc());
	CPPUNIT_ASSERT_EQUAL(int(Utf8Stream::SEOF), us.getc());
	us.ungetc();
	CPPUNIT_ASSERT_EQUAL(int('!'), us.getc());
	CPPUNIT_ASSERT_EQUAL(int(Utf8Stream::SEOF), us.getc());
}

}}
