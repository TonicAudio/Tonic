//
//  TonicTests.h
//  TonicTests
//
//  Created by Nick D on 4/19/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#pragma once
#ifndef TONICTESTS_H
#define TONICTESTS_H


#include <map>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstdlib>

namespace TonicTests {
	// Return Value of a testcase
	enum Result {
		OK, FAILED
	};

	// Test Case
	// ---------
	
	class Testcase {
	public:
		virtual std::string& getName() = 0;
		virtual Result run(std::ostream&) = 0;
		virtual void setUp() {};
		virtual void tearDown() {};

		// Test Functions
		// --------------

		// test if a value is as expected
		template <class T>
		Result test_eq(
			std::ostream& failmsg, const int line, T expected, T value, std::string name) 
		{
			if (expected != value) {
				failmsg << name << " expected to be " << expected << " but is " << value;
				return FAILED;
			}
			else
			{
				return OK;
			}
		}

	// ----------------------------------------------------------------

	// test if value is true
		Result test_true(
			std::ostream& failmsg, const int line, bool value, std::string testname) 
		{
			if (!value) {
				failmsg << testname << " expected to be true but is false";
				return FAILED;
			}
			else
			{
				return OK;
			}
		}	

	// ----------------------------------------------------------------

	// test if value is false
		Result test_false(
			std::ostream& failmsg, const int line, bool value, std::string testname) 
		{
			if (value) {
				failmsg << testname << " expected to be false but is true";
				return FAILED;
			}
			else
			{
				return OK;
			}
		}	
	};
	// ================================================================

	// the testcase registration list
	typedef std::map<int, Testcase*> Testcases;	

	// ================================================================

	// Logging of Testcase output
	// --------------------------

	class TestsuiteBase;
	
	class Logger {
	public:
		// test_start always is followed by a call to the same following object
		virtual void test_start(Testcase& testcase) = 0;
		virtual void test_passed(Testcase& testcase) = 0;
		virtual void test_failed(Testcase& testcase, std::ostringstream& failmsg) = 0;

		// testsuite_start always is followed by a call to the same following object
		virtual void testsuite_start(TestsuiteBase& suite) = 0;
		virtual void testsuite_done(TestsuiteBase& suite) = 0;
	};

	// ================================================================

	// Test Suite
	// ----------

	class TestsuiteBase {
	private:
		Logger& logger;
		std::string name;
	public:
		TestsuiteBase(Logger& mylogger, const char * myname)
		: logger(mylogger)
		, name(myname)
		{}

		std::string& getName() {
			return name;
		}

		// Run Testsuite
		void run(Testcases& tests) {
			logger.testsuite_start(*this);

			for (typename Testcases::iterator i = tests.begin(); i != tests.end(); ++i)
			{
				Testcase* test = i->second;
				std::ostringstream failmsg;

				logger.test_start(*test);

					// run testcase
				test->setUp();
				const Result retval = test->run(failmsg);
				test->tearDown();

				if (retval != OK) {
					logger.test_failed(*test, failmsg);
				} else {
					logger.test_passed(*test);
				}

				delete test;
			}

			logger.testsuite_done(*this);
		}
	};

	template<class TestcaseClass, Testcases** testcaseStorage>
	class Testsuite : public TestsuiteBase {
	public:
		typedef Testsuite<TestcaseClass, testcaseStorage> LocalTestsuite;
		typedef TestcaseClass LocalTestcase;
		
		Testsuite(Logger& mylogger, const char * myname)
		: TestsuiteBase(mylogger, myname)
		{}

		static Testcases& getTests() {
			if (!*testcaseStorage) *testcaseStorage = new Testcases();
			return **testcaseStorage;
		}

    static void addTestcase( Testcase* testcase, const int line ) {
			getTests()[line] = testcase;
		} 

		template <class CurrentTestcase, int Line>
		class TestcaseRegistration {
		public:
			// constructor which in fact registers the testcase
			TestcaseRegistration() {
        LocalTestsuite::addTestcase(new CurrentTestcase(), Line);
			}
		};

		// Run Testsuite
		using TestsuiteBase::run;
		void run() {
			run(getTests());
		}

	};


	// ======================================================================== 


	// Logging unto the Console
	// ------------------------

	class ConsoleLogger : public Logger {
	private:
			int num_tests;
			int passed_tests;
			int failed_tests;

	public:

		ConsoleLogger()
			: num_tests(0)
			, passed_tests(0)
			, failed_tests(0)
		{}

		virtual ~ConsoleLogger() {
			std::cout << num_tests
				<< " tests done: " 
				<< " passed: " << passed_tests 
				<< " failed: " << failed_tests
				<< std::endl;			
		}

		virtual void test_passed(Testcase& test)
		{
			passed_tests++;
			num_tests++;
		}

		virtual void test_failed(Testcase& test, std::ostringstream& failmsg)
		{
			std::cout 
				<< std::right
				<< std::setw(40)
				<< test.getName()
				<< " : "
				<< std::left
				<< std::setw(40)
				<< failmsg.str()
				<< std::endl;
			failed_tests++;
			num_tests++;
		}

		virtual void testsuite_start(TestsuiteBase& suite)
		{
			std::cout << "processing testsuite " << suite.getName() << std::endl;
		}

		virtual void testsuite_done(TestsuiteBase& suite) 	
		{}

		virtual void test_start(Testcase& testcase)
		{}
	};	

} // namespace TonicTests

// ================================================================

// Uniqe Testcase Names
// --------------------

#define TESTCASE_NAME__( name, line ) name##line
#define TESTCASE_NAME_( name, line ) TESTCASE_NAME__( name, line )
#define TESTCASE_NAME( name ) TESTCASE_NAME_( testcase_ ## name, __LINE__ )

// Declare a Testcase
// ------------------

#define TESTCASE(_name)                                                  \
	class TESTCASE_NAME( class );                                          \
	TestcaseRegistration<TESTCASE_NAME( class ), __LINE__>                 \
		TESTCASE_NAME( reg );                                                \
	class TESTCASE_NAME( class ) : public LocalTestcase {                  \
		std::string name;                                                    \
		public:                                                              \
		TESTCASE_NAME( class )() : name(_name) {}                            \
		virtual string& getName() { return name; }                           \
		virtual Result run(std::ostream& failmsg) {                          \

#define END_TESTCASE()                                                   \
			return OK;                                                         \
		}                                                                    \
	};                                                                     \

// Macros for Tests
// ----------------


// call a test function and return upon failure. DRY principle
#define TEST(testtype, ...)  {                                               \
const Result retval = test_##testtype(failmsg, __LINE__, __VA_ARGS__);       \
	if (retval != OK) return retval;                                           \
}

// only keep for reference
#if 0
// call outsourced test function with any argument
#define TEST_OK(function, msg)  {                                            \
	const Result retval = testOK(                                              \
			function(__VA_ARGS__, buf, failmsg) ,msg, failmsg);                    \
	if (retval != OK) return retval;                                           \
}

#define TEST_EQUAL(expected, value, msg) {                                   \
	const Result retval = testEqual(expected, value, msg, failmsg);            \
	if (retval != OK) return retval;                                           \
}

#define TEST_TRUE(value, msg) {                                              \
	const Result retval = testTrue(value, msg, failmsg);                       \
	if (retval != OK) return retval;                                           \
}

#define TEST_FALSE(value, msg)  {                                            \
	const Result retval = testFalse(value, msg, failmsg);                      \
	if (retval != OK) return retval;                                           \
}


#define TEST_FIXEDOUTPUT_EQUALS(l, msg) {                                              \
		const Result retval = testFixedOutputEquals(l, msg, failmsg);                      \
		if (retval != OK) return retval;                                                   \
}

#define TEST_STEREOFIXEDOUTPUT_EQUALS(l, r, msg) {                                     \
		const Result retval = testStereoFixedOutputEquals(l, r, msg, failmsg);             \
		if (retval != OK) return retval;                                                   \
}

#define TEST_BUFFERFILLER_MONOFIXEDOUTPUT_EQUALS(l, msg) {                             \
		const Result retval = testBufferFillerMonoFixedOutputEquals(l, msg, failmsg);      \
		if (retval != OK) return retval;                                                   \
}

#define TEST_BUFFERFILLER_STEREOFIXEDOUTPUT_EQUALS(l, r, msg) {                        \
		const Result retval = testBufferFillerStereoFixedOutputEquals(l, r, msg, failmsg); \
		if (retval != OK) return retval;                                                   \
}
#endif

#endif //TONICTESTS_H