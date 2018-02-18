#include <cppunit/extensions/HelperMacros.h>
#include <jsoncc.h>

#include <jsoncc-cppunit.h>

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
	void test_number_conversions();
	void test_string();

	CPPUNIT_TEST_SUITE(test);
	CPPUNIT_TEST(test_null);
	CPPUNIT_TEST(test_true);
	CPPUNIT_TEST(test_false);
	CPPUNIT_TEST(test_number);
	CPPUNIT_TEST(test_number_conversions);
	CPPUNIT_TEST(test_string);
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

	CPPUNIT_ASSERT(!equal(Json::Number(0), Json::Number()));

	Json::Number n1;
	n1 = n;
	CPPUNIT_ASSERT_EQUAL(n, n1);
	n1 = n1;
	CPPUNIT_ASSERT_EQUAL(n, n1);
}

void test::test_number_conversions()
{
	Json::Number sn1(int8_t(5));
	CPPUNIT_ASSERT_EQUAL(Json::Number::TYPE_INT, sn1.type());
	CPPUNIT_ASSERT_EQUAL(int64_t(5), sn1.int_value());

	Json::Number sn2(int16_t(5));
	CPPUNIT_ASSERT_EQUAL(Json::Number::TYPE_INT, sn2.type());
	CPPUNIT_ASSERT_EQUAL(int64_t(5), sn2.int_value());

	Json::Number sn3(int32_t(5));
	CPPUNIT_ASSERT_EQUAL(Json::Number::TYPE_INT, sn3.type());
	CPPUNIT_ASSERT_EQUAL(int64_t(5), sn3.int_value());

	Json::Number sn4(int64_t(5));
	CPPUNIT_ASSERT_EQUAL(Json::Number::TYPE_INT, sn4.type());
	CPPUNIT_ASSERT_EQUAL(int64_t(5), sn4.int_value());

	CPPUNIT_ASSERT_EQUAL(sn1, sn2);
	CPPUNIT_ASSERT_EQUAL(sn2, sn3);
	CPPUNIT_ASSERT_EQUAL(sn3, sn4);

	Json::Number un1(uint8_t(5));
	CPPUNIT_ASSERT_EQUAL(Json::Number::TYPE_UINT, un1.type());
	CPPUNIT_ASSERT_EQUAL(uint64_t(5), un1.uint_value());

	Json::Number un2(uint16_t(5));
	CPPUNIT_ASSERT_EQUAL(Json::Number::TYPE_UINT, un2.type());
	CPPUNIT_ASSERT_EQUAL(uint64_t(5), un2.uint_value());

	Json::Number un3(uint32_t(5));
	CPPUNIT_ASSERT_EQUAL(Json::Number::TYPE_UINT, un3.type());
	CPPUNIT_ASSERT_EQUAL(uint64_t(5), un3.uint_value());

	Json::Number un4(uint64_t(5));
	CPPUNIT_ASSERT_EQUAL(Json::Number::TYPE_UINT, un4.type());
	CPPUNIT_ASSERT_EQUAL(uint64_t(5), un4.uint_value());

	CPPUNIT_ASSERT_EQUAL(un1, un2);
	CPPUNIT_ASSERT_EQUAL(un2, un3);
	CPPUNIT_ASSERT_EQUAL(un3, un4);

	Json::Number fp1(float(5.0));
	CPPUNIT_ASSERT_EQUAL(Json::Number::TYPE_FP, fp1.type());
	CPPUNIT_ASSERT_EQUAL((long double)(5), fp1.fp_value());

	Json::Number fp2(double(5.0));
	CPPUNIT_ASSERT_EQUAL(Json::Number::TYPE_FP, fp2.type());
	CPPUNIT_ASSERT_EQUAL((long double)(5), fp2.fp_value());

	Json::Number fp3((long double)(5.0));
	CPPUNIT_ASSERT_EQUAL(Json::Number::TYPE_FP, fp3.type());
	CPPUNIT_ASSERT_EQUAL((long double)(5), fp3.fp_value());

	CPPUNIT_ASSERT_EQUAL(fp1, fp2);
	CPPUNIT_ASSERT_EQUAL(fp2, fp3);

	// CAVEAT!
	CPPUNIT_ASSERT(!equal(sn1, un1));
	CPPUNIT_ASSERT(!equal(sn1, fp1));
	CPPUNIT_ASSERT(!equal(un1, fp1));
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
	in.push_back('\\');
	in.push_back('"');
	ss << Json::String(in);

	std::string expected(
		"\""
		"\\u0000\\u0001\\u0002\\u0003\\u0004\\u0005\\u0006\\u0007"
		"\\b\\t\\n\\u000b\\f\\r\\u000e\\u000f"
		"\\u0010\\u0011\\u0012\\u0013\\u0014\\u0015\\u0016\\u0017"
		"\\u0018\\u0019\\u001a\\u001b\\u001c\\u001d\\u001e\\u001f"
		"\\\\"
		"\\\""
		"\""
	);
	CPPUNIT_ASSERT_EQUAL(expected, ss.str());
	ss.str("");

	Json::String s("foo");
	CPPUNIT_ASSERT_EQUAL(s, s);
	CPPUNIT_ASSERT_EQUAL(Json::String("foo"), Json::String("foo"));
	CPPUNIT_ASSERT_EQUAL(Json::String(""), Json::String(""));
	CPPUNIT_ASSERT_EQUAL(Json::String(), Json::String());
	CPPUNIT_ASSERT(!equal(Json::String("Foo"), Json::String("Bar")));
	CPPUNIT_ASSERT(!equal(Json::String("Foo"), Json::String()));

	Json::String s1;
	s1 = s;
	CPPUNIT_ASSERT_EQUAL(s, s1);
	s1 = s1;
	CPPUNIT_ASSERT_EQUAL(s, s1);
}

}}
