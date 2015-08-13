#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TestResult.h>
#include <cppunit/TextTestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/CompilerOutputter.h>

int main()
{
	CppUnit::TextTestRunner runner;
	CppUnit::TestFactoryRegistry & registry(
		CppUnit::TestFactoryRegistry::getRegistry());
	CppUnit::Test *test(registry.makeTest());
	runner.addTest(test);

	CppUnit::BriefTestProgressListener progress;
	runner.eventManager().addListener(&progress);

	runner.setOutputter(new CppUnit::CompilerOutputter(
		&runner.result(), std::cerr, "%p:%l:"));

	return runner.run() ? 0 : 1;
}
