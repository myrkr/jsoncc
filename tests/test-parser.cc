#include <cstring>

#include "error-assert.h"
#include "error-io.h"
#include "parser-impl.h"

namespace unittests {
namespace parser {

class test : public CppUnit::TestCase {
public:
	test();
	void setUp();
	void tearDown();

private:
	void test_empty_document();
	void test_empty_array();
	void test_unbalanced_empty_array();
	void test_simple_array();
	void test_unbalanced_simple_array();
	void test_nested_array();
	void test_unbalanced_nested_array();
	void test_empty_object();
	void test_unbalanced_empty_object();
	void test_simple_object();
	void test_unbalanced_simple_object();
	void test_missing_key_simple_object();
	void test_missing_colon_simple_object();
	void test_missing_value_simple_object();
	void test_missing_seperator_simple_object();
	void test_nested_object();
	void test_unbalanced_nested_object();
	void test_complex();
	void test_max_nesting();

	CPPUNIT_TEST_SUITE(test);
	CPPUNIT_TEST(test_empty_document);
	CPPUNIT_TEST(test_empty_array);
	CPPUNIT_TEST(test_unbalanced_empty_array);
	CPPUNIT_TEST(test_simple_array);
	CPPUNIT_TEST(test_unbalanced_simple_array);
	CPPUNIT_TEST(test_nested_array);
	CPPUNIT_TEST(test_unbalanced_nested_array);
	CPPUNIT_TEST(test_empty_object);
	CPPUNIT_TEST(test_unbalanced_empty_object);
	CPPUNIT_TEST(test_simple_object);
	CPPUNIT_TEST(test_unbalanced_simple_object);
	CPPUNIT_TEST(test_missing_key_simple_object);
	CPPUNIT_TEST(test_missing_colon_simple_object);
	CPPUNIT_TEST(test_missing_value_simple_object);
	CPPUNIT_TEST(test_missing_seperator_simple_object);
	CPPUNIT_TEST(test_nested_object);
	CPPUNIT_TEST(test_unbalanced_nested_object);
	CPPUNIT_TEST(test_complex);
	CPPUNIT_TEST(test_max_nesting);
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

void test::test_empty_document()
{
	ParserImpl parser;

	char data[] = "";
	parser.parse(data, sizeof(data) - 1);

	char data2[] = "              ";
	parser.parse(data2, sizeof(data2) - 1);
}

void test::test_empty_array()
{
	ParserImpl parser;

	char data[] = "[]";
	parser.parse(data, sizeof(data) - 1);

	char data2[] = "   [      ]   ";
	parser.parse(data2, sizeof(data2) - 1);
}

void test::test_unbalanced_empty_array()
{
	ParserImpl parser;

	char data[] = "[";
	jsonp::Error error;
	CPPUNIT_ASSERT_THROW_VAR(
		parser.parse(data, sizeof(data) - 1), jsonp::Error, error);
	CPPUNIT_ASSERT_EQUAL(jsonp::Error::BAD_TOKEN_ARRAY_START, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(0), error.location.offs);
}

void test::test_simple_array()
{
	ParserImpl parser;

	char data[] = "[ true, false, null, \"hello\", 1234]";
	parser.parse(data, sizeof(data) - 1);
}

void test::test_unbalanced_simple_array()
{
	ParserImpl parser;

	char data[] = "[ true,";
	jsonp::Error error;
	CPPUNIT_ASSERT_THROW_VAR(
		parser.parse(data, sizeof(data) - 1), jsonp::Error, error);
	CPPUNIT_ASSERT_EQUAL(jsonp::Error::BAD_TOKEN_ARRAY_NEXT, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(0), error.location.offs);
}

void test::test_nested_array()
{
	ParserImpl parser;

	char data[] = "[[ true ], [[]], [ true, false, [ null, 1 ]]]";
	parser.parse(data, sizeof(data) - 1);
}

void test::test_unbalanced_nested_array()
{
	ParserImpl parser;

	char data[] = "[[ true ], [[], [ true, false, [ null, 1 ]]]";
	jsonp::Error error;
	CPPUNIT_ASSERT_THROW_VAR(
		parser.parse(data, sizeof(data) - 1), jsonp::Error, error);
	CPPUNIT_ASSERT_EQUAL(jsonp::Error::BAD_TOKEN_ARRAY_VALUE, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(0), error.location.offs);
}

void test::test_empty_object()
{
	ParserImpl parser;

	char data[] = "{}";
	parser.parse(data, sizeof(data) - 1);

	char data2[] = "   {      }   ";
	parser.parse(data2, sizeof(data2) - 1);
}

void test::test_unbalanced_empty_object()
{
	ParserImpl parser;

	char data[] = "{";
	jsonp::Error error;
	CPPUNIT_ASSERT_THROW_VAR(
		parser.parse(data, sizeof(data) - 1), jsonp::Error, error);
	CPPUNIT_ASSERT_EQUAL(jsonp::Error::BAD_TOKEN_OBJECT_START, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(0), error.location.offs);
}

void test::test_simple_object()
{
	ParserImpl parser;

	char data[] = "{\n"
	"\"key1\": true,\n"
	"\"key2\": null,\n"
	"\"key3\": 1234,\n"
	"\"key4\": \"hello\"\n"
	"}\n";
	parser.parse(data, sizeof(data) - 1);
}

void test::test_unbalanced_simple_object()
{
	ParserImpl parser;

	char data[] = "{\n"
	"\"key1\": true,\n"
	"\"key2\": null,\n"
	"\"key3\": 1234,\n"
	"\"key4\": \"hello\"\n"
	"\n";

	jsonp::Error error;
	CPPUNIT_ASSERT_THROW_VAR(
		parser.parse(data, sizeof(data) - 1), jsonp::Error, error);
	CPPUNIT_ASSERT_EQUAL(jsonp::Error::BAD_TOKEN_OBJECT_VALUE, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(0), error.location.offs);
}

void test::test_missing_key_simple_object()
{
	ParserImpl parser;

	char data[] = "{\n"
	" : true,\n"
	"\"key2\": null,\n"
	"\"key3\": 1234,\n"
	"\"key4\": \"hello\"\n"
	"}\n";

	jsonp::Error error;
	CPPUNIT_ASSERT_THROW_VAR(
		parser.parse(data, sizeof(data) - 1), jsonp::Error, error);
	CPPUNIT_ASSERT_EQUAL(jsonp::Error::BAD_TOKEN_OBJECT_START, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(0), error.location.offs);
}

void test::test_missing_colon_simple_object()
{
	ParserImpl parser;

	char data[] = "{\n"
	"\"key1\" true,\n"
	"\"key2\": null,\n"
	"\"key3\": 1234,\n"
	"\"key4\": \"hello\"\n"
	"}\n";

	jsonp::Error error;
	CPPUNIT_ASSERT_THROW_VAR(
		parser.parse(data, sizeof(data) - 1), jsonp::Error, error);
	CPPUNIT_ASSERT_EQUAL(jsonp::Error::BAD_TOKEN_OBJECT_NAME, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(0), error.location.offs);
}

void test::test_missing_value_simple_object()
{
	ParserImpl parser;

	char data[] = "{\n"
	"\"key1\": true,\n"
	"\"key2\": ,\n"
	"\"key3\": 1234,\n"
	"\"key4\": \"hello\"\n"
	"}\n";

	jsonp::Error error;
	CPPUNIT_ASSERT_THROW_VAR(
		parser.parse(data, sizeof(data) - 1), jsonp::Error, error);
	CPPUNIT_ASSERT_EQUAL(jsonp::Error::BAD_TOKEN_OBJECT_SEP, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(0), error.location.offs);
}

void test::test_missing_seperator_simple_object()
{
	ParserImpl parser;

	char data[] = "{\n"
	"\"key1\": true,\n"
	"\"key2\": null\n"
	"\"key3\": 1234,\n"
	"\"key4\": \"hello\"\n"
	"}\n";

	jsonp::Error error;
	CPPUNIT_ASSERT_THROW_VAR(
		parser.parse(data, sizeof(data) - 1), jsonp::Error, error);
	CPPUNIT_ASSERT_EQUAL(jsonp::Error::BAD_TOKEN_OBJECT_VALUE, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(0), error.location.offs);
}

void test::test_nested_object()
{
	ParserImpl parser;

	char data[] = "{\n"
	"\"key1\": {},\n"
	"\"key2\": null,\n"
	"\"key3\": {\n"
	"	\"key1\": true,\n"
	"	\"key2\": false\n"
	"},\n"
	"\"key4\": \"hello\"\n"
	"}\n";
	parser.parse(data, sizeof(data) - 1);
}

void test::test_unbalanced_nested_object()
{
	ParserImpl parser;

	char data[] = "{\n"
	"\"key1\": {},\n"
	"\"key2\": null,\n"
	"\"key3\": {\n"
	"	\"key1\": true,\n"
	"	\"key2\": false\n"
	",\n"
	"\"key4\": \"hello\"\n"
	"}\n";
	jsonp::Error error;
	CPPUNIT_ASSERT_THROW_VAR(
		parser.parse(data, sizeof(data) - 1), jsonp::Error, error);
	CPPUNIT_ASSERT_EQUAL(jsonp::Error::BAD_TOKEN_OBJECT_VALUE, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(0), error.location.offs);
}

void test::test_complex()
{
	ParserImpl parser;

	char data[] = "{\n"
	"\"key1\": {},\n"
	"\"key2\": [],\n"
	"\"key3\": {\n"
	"	\"key1\": [true, false, {}],\n"
	"	\"key2\": { \"on\":false, \"off\":true }\n"
	"},\n"
	"\"key4\": [ \"hello\", { \"world\" : null } ]\n"
	"}\n";
	parser.parse(data, sizeof(data) - 1);
}

void test::test_max_nesting()
{
	ParserImpl parser;

	char data[256];
	memset(data, '[', sizeof(data));

	jsonp::Error error;
	CPPUNIT_ASSERT_THROW_VAR(
		parser.parse(data, 256), jsonp::Error, error);
	CPPUNIT_ASSERT_EQUAL(jsonp::Error::PARSER_OVERFLOW, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(0), error.location.offs);
}

}}
