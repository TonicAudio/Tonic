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
	
	class TestcaseBase {
	public:
		virtual std::string& getName() = 0;
		virtual Result run(std::ostream&) = 0;
		virtual void setUp() {};
		virtual void tearDown() {};

		// Test Functions
		// --------------

		// test if a value is as expected
		template <class T>
		Result testEqual(
			T expected, T value, std::string name, std::ostream& failmsg) 
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
		Result testTrue(
			bool value, std::string testname, std::ostream& failmsg) 
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

	// test if value is true
		Result testOK(
			Result value, std::string testname, std::ostream& failmsg) 
		{
			if (value != OK) {
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
		Result testFalse(
			bool value, std::string testname, std::ostream& failmsg) 
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

	// Test Suite
	// ----------

	template<class SuiteTestcaseBase>
	class Testsuite {
	public:
		typedef SuiteTestcaseBase Testcase;

    // the testcase registration list
		typedef std::map<int, TestcaseBase*> Testcases;

		static Testcases* s_tests ;

		static Testcases& getTests() {
			if (!s_tests) {
				s_tests = new Testcases();
			}
			return *s_tests;
		}

    static void addTestcase( TestcaseBase* testcase, const int line ) {
			getTests()[line] = testcase;
		} 

		template <class Testcase, int Line>
		class TestcaseRegistration {
		public:
			// constructor which in fact registers the testcase
			TestcaseRegistration() {
        Testsuite::addTestcase(new Testcase(), Line);
			}
		};

		// ----------------------------------------------------------------


		// ================================================================

		// Run Testsuite
		// -------------

		void run() {
			int num_tests = 0;
			int passed_tests = 0;
			int failed_tests = 0;

			Testcases& tests = getTests();
			for (typename Testcases::iterator i = tests.begin(); i != tests.end(); ++i)
			{
				TestcaseBase* test = i->second;
				num_tests++;
				std::ostringstream failmsg;

					// run testcase
				test->setUp();
				const Result retval = test->run(failmsg);
				test->tearDown();

				if (retval != OK) {
					std::cout << test->getName()
					<< std::setw(20)
					<< " : "
					<< failmsg.str()
					<< std::setw(40)
					<< " : "
					<< ((retval == OK) ? "OK" : "FAILED")
					<< std::endl;
					failed_tests++;
				} else {
					passed_tests++;
				}

				delete test;
			}

			std::cout << num_tests
			<< " tests done: " 
			<< " passed: " << passed_tests 
			<< " failed: " << failed_tests
			<< std::endl;
		}

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

#define TESTCASE(_name)                                     \
	class TESTCASE_NAME( class );                             \
	TestcaseRegistration<TESTCASE_NAME( class ), __LINE__>    \
		TESTCASE_NAME( reg );                                   \
	class TESTCASE_NAME( class ) : public Testcase            \
	{                                                         \
		std::string name;                                       \
		public:                                                 \
		TESTCASE_NAME( class )() : name(_name) {}               \
		virtual string& getName() { return name; }              \
		virtual Result run(std::ostream& failmsg) {             \

#define END_TESTCASE()                                      \
			return OK;                                            \
		}                                                       \
	};                                                        \

// Macros for Tests
// ----------------


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

#endif //TONICTESTS_H