//
//  TonicTests.h
//  TonicTests
//
//  Created by Andreas Koerner 2015-02-06
//  Copyright (c) 2015 Andreas Koerner. All rights reserved.
//

#pragma once
#ifndef TONICTESTS_H
#define TONICTESTS_H

// Configuration
// -------------

// override the namespace name for the header file
#ifndef TONICTESTS_NAMESPACE
#define TONICTESTS_NAMESPACE TonicTests
#endif

// you can supply your custom testsuite baseclass if you #define
// TONICTESTS_BASECLASS. Your Class must implement TestsuiteBase.
#ifndef TONICTESTS_BASECLASS
#define TONICTESTS_BASECLASS BasicTestsuite
#endif

// if set to zero you can provide a name to your testcases TESTCASE(x, "desc")
// instead of TESTCASE("desc")
#ifndef TONICTESTS_AUTONAMES
#define TONICTESTS_AUTONAMES 1
#endif

// skip including the default logger and therefore including I/O headers
#ifndef TONICTESTS_DEFAULT_LOGGER
#define TONICTESTS_DEFAULT_LOGGER 1
#endif

#include <map>
#include <sstream>
#include <string>

#if TONICTESTS_DEFAULT_LOGGER
#include <iostream>
#include <iomanip>
#endif

#include <cstdio>

// ======================================================================== 

// Testsuite Class System
// ----------------------

namespace TONICTESTS_NAMESPACE {
	// Return Value of a testcase
	enum Result {
		OK, FAILED
	};

	// Static Name Formatter
	// ---------------------
	
	class Formatted {
		static const int maxlen = 256;
		char buf[maxlen];

	public:
		operator const char * () { return buf; }
		operator std::string  () { return std::string(buf); }

		template <class T>
		Formatted(const char * fmt, T v1) 
		{ snprintf(buf, maxlen, fmt, v1); }
		template <class T1, class T2>
		Formatted(const char * fmt, T1 v1, T2 v2) 
		{ snprintf(buf, maxlen, fmt, v1, v2); }
	};
	

	// Test Case
	// ---------

	class Testcase {
	public:
		virtual std::string& getName() = 0;
		virtual std::string& getDesc() = 0;
		virtual Result run(std::ostream&) = 0;
		virtual void setUp() {};
		virtual void tearDown() {};

		// Test Functions
		// --------------

		// - a Test function MUST start with the following arguments:
		//     std::ostream& failmsg, const int line
		// - it should end with "std::string name" to note which did go wrong
		//   but this is not imposed by the TEST() macro
		// - you can write arbitrary stuff to failmsg, it only will be written,
		//   when the testcase failed

		// test if a value is as expected
		template <class A, class B>
		Result test_eq(
			std::ostream& failmsg, const int line, A expected, B value, std::string name) 
		{
			if (expected != value) 
				return write_fail(failmsg, line, expected, value, name);
			return OK;
		}

	// ----------------------------------------------------------------

	// test if value is true
		Result test_true(
			std::ostream& failmsg, const int line, bool value, std::string testname) 
		{
			if (!value) 
				return write_fail(failmsg, line, true, value, testname);
			return OK;
		}	

	// ----------------------------------------------------------------

	// test if value is false
		Result test_false(
			std::ostream& failmsg, const int line, bool value, std::string testname) 
		{
			if (value) 
				return write_fail(failmsg, line, false, value, testname);
			return OK;
		}

		//--------------------------------------------------------------------------

		template<class A, class B>
		Result write_fail(
			std::ostream& failmsg, const int line, 
			A expected, B value, std::string& testname) 
		{
			failmsg << testname 
							<< " expected to be " << expected 
							<< " but is " << value
							<< " on line " << line;
			return FAILED;
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
		virtual void test_failed(Testcase& testcase, std::ostringstream& failmsg)=0;

		// testsuite_start always is followed by a call to the same following object
		virtual void testsuite_start(TestsuiteBase& suite) = 0;
		virtual void testsuite_done(TestsuiteBase& suite) = 0;
	};

	// ================================================================

	// Test Suite
	// ----------

	class TestsuiteBase {
	protected:
		Logger& logger;
		std::string name;
		std::string description;
	public:
		TestsuiteBase(Logger& mylogger, const char * myname, const char * mydesc)
		: logger(mylogger)
		, name(myname)
		, description(mydesc)
		{}

		std::string& getName() {
			return name;
		}

		// Run Testsuite
		virtual void run(Testcases& tests) = 0;
	};	

	class BasicTestsuite : public TestsuiteBase {
	public:
		BasicTestsuite(Logger& mylogger, const char * myname, const char * mydesc)
		: TestsuiteBase(mylogger, myname, mydesc)
		{}

		// Run Testsuite
		virtual void run(Testcases& tests) {
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

	//--------------------------------------------------------------------------

	template<class TestcaseClass, Testcases*& testcaseStorage>
	class Testsuite : public TONICTESTS_BASECLASS {
	public:
		typedef Testsuite<TestcaseClass, testcaseStorage> LocalTestsuite;
		typedef TestcaseClass LocalTestcase;
		
		Testsuite(Logger& mylogger, const char * myname, const char * mydesc)
		: TONICTESTS_BASECLASS(mylogger, myname, mydesc)
		{}

		static Testcases& getTests() {
			if (!testcaseStorage) testcaseStorage = new Testcases();
			return *testcaseStorage;
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
		using BasicTestsuite::run;
		void run() {
			run(getTests());
		}
	};


	// ======================================================================== 

	// Logging unto the Console
	// ------------------------
	
	#if TONICTESTS_DEFAULT_LOGGER

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
					#if TONICTESTS_AUTONAMES
					<< std::setw(40)
					<< test.getDesc()
					#else
					<< std::setw(20)
					<< test.getName()
					#endif
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

	#endif //TONICTESTS_DEFAULT_LOGGER

} // namespace TONICTESTS_NAMESPACE

// ================================================================

// Uniqe Testcase Names
// --------------------

#define TONICTEST_NAME__( name, line ) name##line
#define TONICTEST_NAME_( name, line )  TONICTEST_NAME__( name, line )
#define TONICTEST_NAME( name )         TONICTEST_NAME_( name, __LINE__ )

// Define a Testsuite
// ------------------

// forward macro to eventually adapt macro arguments
#define TESTSUITE(_name, _testcase, _desc) TESTSUITE_(_name, _testcase, _desc)

#define TESTSUITE_(_name, _testcase, _desc)                                    \
	TONICTESTS_NAMESPACE::Testcases* _name##List = 0;                            \
	typedef TONICTESTS_NAMESPACE::Testsuite<_testcase,_name##List> _name##Base;  \
	class _name : public _name##Base                                             \
	{                                                                            \
	public:                                                                      \
	  _name(Logger& mylogger)                                                    \
	  : _name##Base(mylogger, #_name, _desc)                                     \
	  {}                                                                         \
	private:                                                                     \

#define END_TESTSUITE() };


// Define a Testcase
// -----------------

// forward macros enable the user to decide wheter to use automatic testcase
// class names or not
#if TONICTESTS_AUTONAMES
#define TESTCASE(_desc) TESTCASE__( TONICTEST_NAME( testcase_ ), _desc )
// resolving TONICTEST_NAME before passing the name to TESTSUITE_
#define TESTCASE__(_name, _desc) TESTCASE_(_name, _desc)
#else
#define TESTCASE(_name, _desc) TESTCASE_(_name, _desc)
#endif

#define TESTCASE_(_name, _desc)                                                \
	class _name;                                                                 \
	TestcaseRegistration<_name, __LINE__> _name##Reg;                            \
	class _name : public LocalTestcase {                                         \
		std::string desc;                                                          \
		std::string name;                                                          \
		public:                                                                    \
		_name() : name(#_name), desc(_desc) {}                                     \
		virtual string& getName() { return name; }                                 \
		virtual string& getDesc() { return desc; }                                 \
		virtual Result run(std::ostream& failmsg) {                                \

#define END_TESTCASE()                                                         \
			return OK;                                                               \
		}                                                                          \
	};                                                                           \

// Perform Tests
// -------------

// call a test function and return upon failure. DRY principle
#define TEST(testtype, ...)  {                                                 \
	const Result retval = test_##testtype(failmsg, __LINE__, __VA_ARGS__);       \
	if (retval != OK) return retval;                                             \
}

#endif //TONICTESTS_H
