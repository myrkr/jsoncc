#include <cstring>

#include "equality.h"
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
	void test_bad_doc_token();
	void test_double_doc();
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
	void test_missing_next_key_object();
	void test_nested_object();
	void test_unbalanced_nested_object();
	void test_complex();
	void test_max_nesting();

	CPPUNIT_TEST_SUITE(test);
	CPPUNIT_TEST(test_empty_document);
	CPPUNIT_TEST(test_bad_doc_token);
	CPPUNIT_TEST(test_double_doc);
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
	CPPUNIT_TEST(test_missing_next_key_object);
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

using namespace Json;

void test::test_empty_document()
{
	ParserImpl parser;
	Json::Value value;

	char data[] = "";
	CPPUNIT_ASSERT_EQUAL(
		Json::Value(),
		parser.parse(data, sizeof(data) - 1)
	);

	char data2[] = "              ";
	CPPUNIT_ASSERT_EQUAL(
		Json::Value(),
		parser.parse(data2, sizeof(data2) - 1)
	);
}

void test::test_bad_doc_token()
{
	ParserImpl parser;

	char data[] = "true";
	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(
		parser.parse(data, sizeof(data) - 1), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::BAD_TOKEN_DOCUMENT, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(0), error.location.offs);
}

void test::test_double_doc()
{
	ParserImpl parser;

	char data[] = "[][]";
	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(
		parser.parse(data, sizeof(data) - 1), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::BAD_TOKEN_DOCUMENT, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(0), error.location.offs);
}

void test::test_empty_array()
{
	ParserImpl parser;

	char data[] = "[]";
	CPPUNIT_ASSERT_EQUAL(
		Json::Value(Json::Array()),
		parser.parse(data, sizeof(data) - 1)
	);

	char data2[] = "   [      ]   ";
	CPPUNIT_ASSERT_EQUAL(
		Json::Value(Json::Array()),
		parser.parse(data2, sizeof(data2) - 1)
	);
}

void test::test_unbalanced_empty_array()
{
	ParserImpl parser;

	char data[] = "[";
	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(
		parser.parse(data, sizeof(data) - 1), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::BAD_TOKEN_ARRAY_START, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(0), error.location.offs);
}

void test::test_simple_array()
{
	ParserImpl parser;

	Json::Array expected;
	expected << true << false << Json::Null() << "hello" << 1234;

	char data[] = "[ true, false, null, \"hello\", 1234]";
	CPPUNIT_ASSERT_EQUAL(
		Json::Value(expected),
		parser.parse(data, sizeof(data) - 1)
	);
}

void test::test_unbalanced_simple_array()
{
	ParserImpl parser;

	char data[] = "[ true,";
	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(
		parser.parse(data, sizeof(data) - 1), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::BAD_TOKEN_ARRAY_NEXT, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(0), error.location.offs);
}

void test::test_nested_array()
{
	ParserImpl parser;

	Json::Array expected;
	expected << (Json::Array() << true)
		 << (Json::Array() << Json::Array())
		 << (Json::Array()
			<< true
			<< false
			<< (Json::Array()
				<< Json::Null()
				<< 1));

	char data[] = "[[ true ], [[]], [ true, false, [ null, 1 ]]]";
	CPPUNIT_ASSERT_EQUAL(
		Json::Value(expected),
		parser.parse(data, sizeof(data) - 1)
	);
}

void test::test_unbalanced_nested_array()
{
	ParserImpl parser;

	char data[] = "[[ true ], [[], [ true, false, [ null, 1 ]]]";
	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(
		parser.parse(data, sizeof(data) - 1), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::BAD_TOKEN_ARRAY_VALUE, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(0), error.location.offs);
}

void test::test_empty_object()
{
	ParserImpl parser;

	char data[] = "{}";
	CPPUNIT_ASSERT_EQUAL(
		Json::Value(Json::Object()),
		parser.parse(data, sizeof(data) - 1)
	);

	char data2[] = "   {      }   ";
	CPPUNIT_ASSERT_EQUAL(
		Json::Value(Json::Object()),
		parser.parse(data2, sizeof(data2) - 1)
	);
}

void test::test_unbalanced_empty_object()
{
	ParserImpl parser;

	char data[] = "{";
	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(
		parser.parse(data, sizeof(data) - 1), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::BAD_TOKEN_OBJECT_START, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(0), error.location.offs);
}

void test::test_simple_object()
{
	ParserImpl parser;

	Json::Object expected;
	expected
		<< Json::Member("key1", true)
		<< Json::Member("key2", Json::Null())
		<< Json::Member("key3", 1234)
		<< Json::Member("key4", "hello");

	char data[] = "{\n"
	"\"key1\": true,\n"
	"\"key2\": null,\n"
	"\"key3\": 1234,\n"
	"\"key4\": \"hello\"\n"
	"}\n";

	CPPUNIT_ASSERT_EQUAL(
		Json::Value(expected),
		parser.parse(data, sizeof(data) - 1)
	);
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

	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(
		parser.parse(data, sizeof(data) - 1), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::BAD_TOKEN_OBJECT_VALUE, error.type);
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

	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(
		parser.parse(data, sizeof(data) - 1), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::BAD_TOKEN_OBJECT_START, error.type);
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

	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(
		parser.parse(data, sizeof(data) - 1), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::BAD_TOKEN_OBJECT_NAME, error.type);
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

	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(
		parser.parse(data, sizeof(data) - 1), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::BAD_TOKEN_OBJECT_SEP, error.type);
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

	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(
		parser.parse(data, sizeof(data) - 1), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::BAD_TOKEN_OBJECT_VALUE, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(0), error.location.offs);
}

void test::test_missing_next_key_object()
{
	ParserImpl parser;

	char data[] = "{\n"
	"\"key1\": true,\n"
	"false\n"
	"}\n";

	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(
		parser.parse(data, sizeof(data) - 1), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::BAD_TOKEN_OBJECT_NEXT, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(0), error.location.offs);
}

void test::test_nested_object()
{
	ParserImpl parser;

	Json::Object expected;
	expected
		<< Json::Member("key1", Json::Object())
		<< Json::Member("key2", Json::Null())
		<< Json::Member("key3", (
			Json::Object()
				<< Json::Member("key1", true)
				<< Json::Member("key2", false)))
		<< Json::Member("key4", "hello");

	char data[] = "{\n"
	"\"key1\": {},\n"
	"\"key2\": null,\n"
	"\"key3\": {\n"
	"	\"key1\": true,\n"
	"	\"key2\": false\n"
	"},\n"
	"\"key4\": \"hello\"\n"
	"}\n";

	CPPUNIT_ASSERT_EQUAL(
		Json::Value(expected),
		parser.parse(data, sizeof(data) - 1)
	);
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
	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(
		parser.parse(data, sizeof(data) - 1), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::BAD_TOKEN_OBJECT_VALUE, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(0), error.location.offs);
}

void test::test_complex()
{
	ParserImpl parser;

	Json::Object expected;
	expected
		<< Json::Member("key1", Json::Object())
		<< Json::Member("key2", Json::Array())
		<< Json::Member("key3", (
			Json::Object()
				<< Json::Member("key1", (
					Json::Array()
						<< true
						<< false
						<< Json::Object()))
				<< Json::Member("key2", (
					Json::Object()
						<< Json::Member("on", false)
						<< Json::Member("off", true)))))
		<< Json::Member("key4", (
			Json::Array()
				<< "hello"
				<< (Json::Object()
					<< Json::Member("world", Json::Null()))));

	char data[] = "{\n"
	"\"key1\": {},\n"
	"\"key2\": [],\n"
	"\"key3\": {\n"
	"	\"key1\": [true, false, {}],\n"
	"	\"key2\": { \"on\":false, \"off\":true }\n"
	"},\n"
	"\"key4\": [ \"hello\", { \"world\" : null } ]\n"
	"}\n";

	CPPUNIT_ASSERT_EQUAL(
		Json::Value(expected),
		parser.parse(data, sizeof(data) - 1)
	);
}

void test::test_max_nesting()
{
	ParserImpl parser;

	char data[256];
	memset(data, '[', sizeof(data));

	Json::Error error;
	CPPUNIT_ASSERT_THROW_VAR(
		parser.parse(data, 256), Json::Error, error);
	CPPUNIT_ASSERT_EQUAL(Json::Error::PARSER_OVERFLOW, error.type);
	CPPUNIT_ASSERT_EQUAL(size_t(0), error.location.offs);
}

}}
