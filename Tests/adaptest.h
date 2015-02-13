//
//  AdapTest.h
//  AdapTest
//
//  Created by Andreas Koerner, Feb 2015
//
// This is free and unencumbered software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
// 
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
// 
// For more information, please refer to <http://unlicense.org>



#pragma once
#ifndef ADAPTEST_H
#define ADAPTEST_H

// Configuration
// -------------

// override the namespace name for the header file
#ifndef ADAPTEST_NAMESPACE
#define ADAPTEST_NAMESPACE AdapTest
#endif

// you can supply your custom testsuite baseclass if you #define
// ADAPTEST_BASECLASS. Your Class must implement TestsuiteBase.
#ifndef ADAPTEST_BASECLASS
#define ADAPTEST_BASECLASS BasicTestsuite
#endif

// if set to one you do not need to provide a name to your testcases aka
// TESTCASE("desc") instead of TESTCASE(name, "desc"). this has the drawback
// of not being able to easily set breakpoints using gdb.
#ifndef ADAPTEST_AUTONAMES
#define ADAPTEST_AUTONAMES 0
#endif

// skip including the default logger and therefore including I/O headers
#ifndef ADAPTEST_DEFAULT_LOGGER
#define ADAPTEST_DEFAULT_LOGGER 1
#endif

// length of Formatted buffer
#ifndef ADAPTEST_FORMATED_BUFLEN
#define ADAPTEST_FORMATED_BUFLEN 256
#endif

#include <map>
#include <sstream>
#include <string>

#if ADAPTEST_DEFAULT_LOGGER
#include <iostream>
#include <iomanip>
#endif

#include <cstdio>

// ======================================================================== 

// Testsuite Class System
// ----------------------

namespace ADAPTEST_NAMESPACE {
  // Return Value of a testcase
  enum Result {
    OK, FAILED, ERROR
  };

  // Static Name Formatter
  // ---------------------
  
  class Formatted {
    static const int buflen = ADAPTEST_FORMATED_BUFLEN;
    char buf[buflen];

  public:
    operator const char * () { return buf; }
    operator std::string  () { return std::string(buf); }

    template <class T>
    Formatted(const char * fmt, T v1) 
    { snprintf(buf, buflen, fmt, v1); }
    template <class T1, class T2>
    Formatted(const char * fmt, T1 v1, T2 v2) 
    { snprintf(buf, buflen, fmt, v1, v2); }
  };
  
  // ================================================================

  // Logging of Testcase output
  // --------------------------

  class TestsuiteBase;
  class Testcase;
  
  class Logger {
  public:
    // test_start always is followed by a call to the same following object
    virtual void test_start(Testcase& testcase) = 0;
    virtual void test_passed(Testcase& testcase) = 0;
    virtual void test_failed(Testcase& testcase, std::ostringstream& msg)=0;
    virtual void test_error(Testcase& testcase, std::ostringstream& errmsg)=0;

    // testsuite_start always is followed by a call to the same following object
    virtual void testsuite_start(TestsuiteBase& suite) = 0;
    virtual void testsuite_done(TestsuiteBase& suite) = 0;

    virtual int getFailed() = 0;
  };

  // ================================================================   

  // Test Case
  // ---------

  class Testcase {
  public:
    virtual std::string& getName() = 0;
    virtual std::string& getDesc() = 0;
    virtual Result run(std::ostream&) = 0;
    virtual void setUp() {}
    virtual void tearDown() {}
    virtual ~Testcase() {}

    // Test Functions
    // --------------

    // - a Test function MUST start with the following arguments:
    //     std::ostream& msg, const int line
    // - it should end with "std::string name" to note which did go wrong
    //   but this is not imposed by the TEST() macro
    // - you can write arbitrary stuff to msg, it only will be written,
    //   when the testcase failed

    // test if a value is as expected
    template <class A, class B>
    Result test_eq(
      std::ostream& msg, const int line, A expected, B value, std::string name) 
    {
      if (expected != value) 
        return fail(msg, line, expected, value, name);
      return OK;
    }

  // ----------------------------------------------------------------

  // test if value is true
    Result test_true(
      std::ostream& msg, const int line, bool value, std::string testname) 
    {
      if (!value) 
        return fail(msg, line, true, value, testname);
      return OK;
    } 

  // ----------------------------------------------------------------

  // test if value is false
    Result test_false(
      std::ostream& msg, const int line, bool value, std::string testname) 
    {
      if (value) 
        return fail(msg, line, false, value, testname);
      return OK;
    }

    //--------------------------------------------------------------------------

    template<class A, class B>
    Result fail(
      std::ostream& msg, const int line, 
      A expected, B value, std::string& testname) 
    {
      msg << testname 
          << " expected to be " << expected 
          << " but is " << value
          << " on line " << line;
      return FAILED;
    }

    //--------------------------------------------------------------------------

    Result error(
      std::ostream& msg, const int line, 
      std::string& errmsg) 
    {
      msg << errmsg;
      return ERROR;
    }
  };
  
  // ================================================================

  // the testcase registration list
  typedef std::map<int, Testcase*> Testcases; 

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

      for (Testcases::iterator i = tests.begin(); i != tests.end(); ++i)
      {
        Testcase* test = i->second;
        std::ostringstream msg;

        logger.test_start(*test);

          // run testcase
        test->setUp();
        const Result retval = test->run(msg);
        test->tearDown();

        if (retval == FAILED) {
          logger.test_failed(*test, msg);
        } else if (retval == ERROR) {
          logger.test_error(*test, msg);
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
  class Testsuite : public ADAPTEST_BASECLASS {
  public:
    typedef Testsuite<TestcaseClass, testcaseStorage> LocalTestsuite;
    typedef TestcaseClass LocalTestcase;
    
    Testsuite(Logger& mylogger, const char * myname, const char * mydesc)
    : ADAPTEST_BASECLASS(mylogger, myname, mydesc)
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
  
  #if ADAPTEST_DEFAULT_LOGGER

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

      virtual void test_failed(Testcase& test, std::ostringstream& msg)
      {
        std::cout 
          << std::right
          #if ADAPTEST_AUTONAMES
          << std::setw(40)
          << test.getDesc()
          #else
          << std::setw(20)
          << test.getName()
          #endif
          << " : "
          << std::left
          << std::setw(40)
          << msg.str()
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

      virtual int getFailed()
      { return failed_tests; }

      virtual void test_error(Testcase& test, std::ostringstream& msg)
      {
        std::cout 
          << "ERROR : "
          << std::right
          #if ADAPTEST_AUTONAMES
          << test.getDesc()
          #else
          << test.getName()
          #endif
          << " : "
          << std::left
          << std::setw(40)
          << msg.str()
          << std::endl;
        failed_tests++;
        num_tests++;
      }
    };  

  #endif //ADAPTEST_DEFAULT_LOGGER

} // namespace ADAPTEST_NAMESPACE

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
  ADAPTEST_NAMESPACE::Testcases* _name##List = 0;                              \
  typedef ADAPTEST_NAMESPACE::Testsuite<_testcase,_name##List> _name##Base;    \
  class _name : public _name##Base                                             \
  {                                                                            \
  public:                                                                      \
    _name(ADAPTEST_NAMESPACE::Logger& mylogger)                                \
    : _name##Base(mylogger, #_name, _desc)                                     \
    {}                                                                         \
  private:                                                                     \

#define END_TESTSUITE() };


// Define a Testcase
// -----------------

// forward macros enable the user to decide wheter to use automatic testcase
// class names or not
#if ADAPTEST_AUTONAMES == 1
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
    virtual std::string& getName() { return name; }                            \
    virtual std::string& getDesc() { return desc; }                            \
    virtual ADAPTEST_NAMESPACE::Result run(std::ostream& msg) {                \

#define END_TESTCASE()                                                         \
      return ADAPTEST_NAMESPACE::OK;                                           \
    }                                                                          \
  };                                                                           \

// Perform Tests
// -------------

// call a test function and return upon failure. DRY principle
#define TEST(testtype, ...)  {                                                 \
  const ADAPTEST_NAMESPACE::Result retval =                                    \
    test_##testtype(msg, __LINE__, __VA_ARGS__);                               \
  if (retval != ADAPTEST_NAMESPACE::OK) return retval;                         \
}

#endif //ADAPTEST_H
