#include <cppunit/extensions/HelperMacros.h>
#include <jsoncc.h>

#include <jsoncc-cppunit.h>

namespace unittests {
namespace json {
namespace array {

class test : public CppUnit::TestCase {
public:
	test();
	void setUp();
	void tearDown();

private:
	void test_empty();
	void test_numbers();
	void test_nested();
	void test_nested_noindent();
	void test_equality();
	void test_from_container();
	void test_iterators();
	void test_list_initialization();
	void test_move();
	void test_vector();
	void test_vector_nested();
	void test_list();
	void test_set();

	CPPUNIT_TEST_SUITE(test);
	CPPUNIT_TEST(test_empty);
	CPPUNIT_TEST(test_numbers);
	CPPUNIT_TEST(test_nested);
	CPPUNIT_TEST(test_nested_noindent);
	CPPUNIT_TEST(test_equality);
	CPPUNIT_TEST(test_from_container);
	CPPUNIT_TEST(test_iterators);
	CPPUNIT_TEST(test_list_initialization);
	CPPUNIT_TEST(test_move);
	CPPUNIT_TEST(test_vector);
	CPPUNIT_TEST(test_vector_nested);
	CPPUNIT_TEST(test_list);
	CPPUNIT_TEST(test_set);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(test);

test::test()
{ }

void test::setUp()
{ }

void test::tearDown()
{ }

void test::test_empty()
{
	std::stringstream ss;
	ss << Json::Array();
	CPPUNIT_ASSERT_EQUAL(std::string("[]"), ss.str());
}

void test::test_numbers()
{
	std::stringstream ss;

	Json::Array a;
	a << 1 << 2 << 3 << 4;
	ss << a;

	std::string expected(
		"[\n"
		"	1,\n"
		"	2,\n"
		"	3,\n"
		"	4\n"
		"]"
	);
	CPPUNIT_ASSERT_EQUAL(expected, ss.str());
}

void test::test_nested()
{
	std::stringstream ss;

	Json::Array a;
	Json::Array a1;
	a1 << 1 << 2;
	Json::Array a2;
	a2 << true << false;
	a << a1;
	a << a2;
	a << Json::Array();
	ss << a;

	std::string expected(
		"[\n"
		"	[\n"
		"		1,\n"
		"		2\n"
		"	],\n"
		"	[\n"
		"		true,\n"
		"		false\n"
		"	],\n"
		"	[]\n"
		"]"
	);
	CPPUNIT_ASSERT_EQUAL(expected, ss.str());
}

void test::test_nested_noindent()
{
	std::stringstream ss;

	Json::Array a;
	Json::Array a1;
	a1 << 1 << 2;
	Json::Array a2;
	a2 << true << false;
	a << a1;
	a << a2;
	a << Json::Array();
	ss << Json::noindent << a;

	std::string expected(
		"[[1, 2], [true, false], []]"
	);
	CPPUNIT_ASSERT_EQUAL(expected, ss.str());
}

void test::test_equality()
{
	CPPUNIT_ASSERT_EQUAL(Json::Array(), Json::Array());
	Json::Array a1;
	CPPUNIT_ASSERT_EQUAL(a1, a1);
	a1 << 1 << 2 << 3;
	Json::Array a2(a1);
	CPPUNIT_ASSERT_EQUAL(a1, a2);
	Json::Array a3;
	a3 << 1 << 2;
	CPPUNIT_ASSERT(!equal(a3, a1));
	CPPUNIT_ASSERT(!equal(a3, Json::Array()));
	Json::Array a4;
	a1 << 3 << 2 << 1;
	CPPUNIT_ASSERT(!equal(a4, a1));
	Json::Array a5;
	a5 = a4;
	CPPUNIT_ASSERT_EQUAL(a4, a5);
	a5 = a5;
	CPPUNIT_ASSERT_EQUAL(a4, a5);
}

void test::test_from_container()
{
	std::vector<uint32_t> v{1, 2, 3};

	Json::Array a1(v.begin(), v.end());
	CPPUNIT_ASSERT_EQUAL(size_t(3), a1.elements().size());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, a1.elements()[0].tag());
	CPPUNIT_ASSERT_EQUAL(uint64_t(1), a1.elements()[0].number().uint_value());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, a1.elements()[1].tag());
	CPPUNIT_ASSERT_EQUAL(uint64_t(2), a1.elements()[1].number().uint_value());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, a1.elements()[2].tag());
	CPPUNIT_ASSERT_EQUAL(uint64_t(3), a1.elements()[2].number().uint_value());

	std::set<std::string> s;
	s.insert("foo");
	s.insert("bar");
	s.insert("baz");

	Json::Array a2(s.begin(), s.end());
	CPPUNIT_ASSERT_EQUAL(size_t(3), a2.elements().size());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_STRING, a2.elements()[0].tag());
	CPPUNIT_ASSERT_EQUAL(std::string("bar"), a2.elements()[0].string().value());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_STRING, a2.elements()[1].tag());
	CPPUNIT_ASSERT_EQUAL(std::string("baz"), a2.elements()[1].string().value());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_STRING, a2.elements()[2].tag());
	CPPUNIT_ASSERT_EQUAL(std::string("foo"), a2.elements()[2].string().value());

	std::list<int32_t> l{1, 2, 3};

	Json::Array a3(l.begin(), l.end());
	CPPUNIT_ASSERT_EQUAL(size_t(3), a3.elements().size());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, a3.elements()[0].tag());
	CPPUNIT_ASSERT_EQUAL(int64_t(1), a3.elements()[0].number().int_value());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, a3.elements()[1].tag());
	CPPUNIT_ASSERT_EQUAL(int64_t(2), a3.elements()[1].number().int_value());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, a3.elements()[2].tag());
	CPPUNIT_ASSERT_EQUAL(int64_t(3), a3.elements()[2].number().int_value());
}

void test::test_iterators()
{
	std::vector<uint32_t> v{1, 2, 3};

	Json::Array a1(v.begin(), v.end());
	for (auto & v: a1) {
		CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, v.tag());
	}
}

void test::test_list_initialization()
{
	Json::Array a1{ 1, 2, 3 };
	for (auto & v: a1) {
		CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, v.tag());
	}

	Json::Array a2{ "foo", 2, true };
	auto it = a2.begin();
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_STRING, it->tag());
	++it;
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, it->tag());
	++it;
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_TRUE, it->tag());
	++it;
	CPPUNIT_ASSERT_EQUAL(a2.end(), it);
}

void test::test_move()
{
	Json::Array a1{ "foo", 2, true };

	Json::Array a2(std::move(a1));
	CPPUNIT_ASSERT(a1.elements().empty());

	{
		auto it = a2.begin();
		CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_STRING, it->tag());
		++it;
		CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, it->tag());
		++it;
		CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_TRUE, it->tag());
		++it;
		CPPUNIT_ASSERT_EQUAL(a2.end(), it);
	}

	Json::Array a3;
	a3 = std::move(a2);
	CPPUNIT_ASSERT(a2.elements().empty());

	{
		auto it = a3.begin();
		CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_STRING, it->tag());
		++it;
		CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, it->tag());
		++it;
		CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_TRUE, it->tag());
		++it;
		CPPUNIT_ASSERT_EQUAL(a3.end(), it);
	}
}

void test::test_vector()
{
	std::stringstream ss;
	std::vector<int> v{1, 2, 3, 4};
	ss << Json::Value(v);

	std::string expected(
		"[\n"
		"	1,\n"
		"	2,\n"
		"	3,\n"
		"	4\n"
		"]"
	);
	CPPUNIT_ASSERT_EQUAL(expected, ss.str());
}

void test::test_vector_nested()
{
	std::stringstream ss;
	std::vector<std::vector<int> > v;
	std::vector<int> v1{1, 2};
	v.push_back(v1);
	v.push_back(v1);
	v.push_back(std::vector<int>());
	ss << Json::Value(v);

	std::string expected(
		"[\n"
		"	[\n"
		"		1,\n"
		"		2\n"
		"	],\n"
		"	[\n"
		"		1,\n"
		"		2\n"
		"	],\n"
		"	[]\n"
		"]"
	);
	CPPUNIT_ASSERT_EQUAL(expected, ss.str());
}

void test::test_list()
{
	std::stringstream ss;
	std::list<int> v{1, 2, 3, 4};
	ss << Json::Value(v);

	std::string expected(
		"[\n"
		"	1,\n"
		"	2,\n"
		"	3,\n"
		"	4\n"
		"]"
	);
	CPPUNIT_ASSERT_EQUAL(expected, ss.str());
}

void test::test_set()
{
	std::stringstream ss;
	std::set<int> v{1, 2, 3, 4};
	ss << Json::Value(v);

	std::string expected(
		"[\n"
		"	1,\n"
		"	2,\n"
		"	3,\n"
		"	4\n"
		"]"
	);
	CPPUNIT_ASSERT_EQUAL(expected, ss.str());
}

}}}
