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
#include <list>
#include <sstream>
#include <string>

#if ADAPTEST_DEFAULT_LOGGER
#include <iostream>
#include <iomanip>
#endif

#include <cstdio>

using std::string;
using std::stringstream;

// ======================================================================== 

// Testsuite Class System
// ----------------------

namespace ADAPTEST_NAMESPACE {
  
  // Testcase Result
  // ---------------

  enum ResultEnum {
    OK, FAILED, ERROR
  };

  struct Result
  {
    ResultEnum resval;
    string test;
    int line;
    string msg;

    Result(ResultEnum _resval, string _test, int _line, string _msg)
    : resval(_resval) , test(_test) , line(_line) , msg(_msg)
    {}

    Result(ResultEnum _resval)
    : resval(_resval), test(""), line(0), msg("")
    {}

    bool operator == (ResultEnum o) { return resval == 0; }
    bool operator != (ResultEnum o) { return resval != 0; }
  };

  // Simple String Formatter
  // -----------------------

  template <class A, class B, class C, class D, class E>
  string 
  format(string fmt, const A& a, const B& b, const C& c, const D& d, const E& e) 
  {
    stringstream output;
    size_t offset = 0;
    size_t param  = -1;
    size_t find   = 0;
    while ((find = fmt.find('{', offset)) != string::npos) {
        output << fmt.substr(offset, find - offset);
        offset = find + 1;
        char selection = fmt[offset];
        if ((selection >= '0') && (selection < '5')) {
            offset++;
            param = (size_t) selection - '0';
        } else if (selection == '}') {
            param++;
        } else {
            offset = fmt.find('}', offset) + 1;
            continue;
        }
        switch (param) {
            case 0: output << a; break;
            case 1: output << b; break;
            case 2: output << c; break;
            case 3: output << d; break;
            case 4: output << e; break;
            default: break;
        }
        offset++;
    }
    output << fmt.substr(offset, fmt.length() - offset);
    return output.str();
  }
  
  template <class A>
  string format(string fmt, const A& a)
  { return format(fmt, a, "", "", "", ""); }

  template <class A, class B>
  string format(string fmt, const A& a, const B& b)
  { return format(fmt, a, b, "", "", ""); }

  template <class A, class B, class C>
  string format(string fmt, const A& a, const B& b, const C& c)
  { return format(fmt, a, b, c, "", ""); }

  template <class A, class B, class C, class D>
  string format(string fmt, const A& a, const B& b, const C& c, const D& d)
  { return format(fmt, a, b, c, d, ""); }

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
    virtual void test_failed(Testcase& testcase, Result& res)=0;
    virtual void test_error(Testcase& testcase,  Result& res)=0;

    // testsuite_start always is followed by a call to the same following object
    virtual void testsuite_start(TestsuiteBase& suite) = 0;
    virtual void testsuite_done(TestsuiteBase& suite) = 0;

    virtual int getFailed() = 0;
  };

  // ================================================================   
    

  // Test Case
  // ---------

  class Testcase {
  private:
    TestsuiteBase* testsuite;
  public:
    virtual std::string& getName() = 0;
    virtual std::string& getDesc() = 0;
    virtual Result run() = 0;
    virtual void setUp() {}
    virtual void tearDown() {}
    virtual ~Testcase() {}
    void setTestsuite(TestsuiteBase& _testsuite) { testsuite = &_testsuite;}
    TestsuiteBase& getTestsuite() { return *testsuite; }


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
      A expected, B value, std::string name, const int line) 
    {
      if (expected != value) 
        return fail(expected, value, name, line);
      return OK;
    }

  // ----------------------------------------------------------------

  // test if value is true
    Result test_true(
      bool value, std::string testname, const int line) 
    {
      if (!value) 
        return fail(true, value, testname, line);
      return OK;
    } 

  // ----------------------------------------------------------------

  // test if value is false
    Result test_false(
      bool value, std::string testname, const int line) 
    {
      if (value) 
        return fail(false, value, testname, line);
      return OK;
    }

    //--------------------------------------------------------------------------

    template<class A, class B>
    Result fail(
      A expected, B value, std::string& testname, const int line) 
    {
      return Result(FAILED, testname, line, 
                    format("{} expected to be {}, but is {}", 
                           testname, expected, value));
    }

    //--------------------------------------------------------------------------

    Result error(
      std::string& errmsg, const int line) 
    {
      return Result(ERROR, "", line, errmsg);
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
    std::string name;
    std::string description;
  public:
    TestsuiteBase(const char * myname, const char * mydesc)
    : name(myname)
    , description(mydesc)
    {}

    std::string& getName()          { return name; }

    // Run Testsuite
    void run_tests(Testcases& tests, Logger& logger) {
      logger.testsuite_start(*this);

      for (Testcases::iterator i = tests.begin(); i != tests.end(); ++i)
      {
        Testcase* test = i->second;

        logger.test_start(*test);

        // run testcase
        test->setTestsuite(*this);
        test->setUp();
        Result retval = test->run();
        test->tearDown();

        if (retval.resval == FAILED) {
          logger.test_failed(*test, retval);
        } else if (retval.resval == ERROR) {
          logger.test_error(*test, retval);
        } else {
          logger.test_passed(*test);
        }

        delete test;
      }

      logger.testsuite_done(*this);
    }

    virtual void run(Logger& logger) = 0;
  };

  //--------------------------------------------------------------------------

  template<class TestcaseClass, Testcases*& testcaseStorage>
  class Testsuite : public TestsuiteBase {
  public:
    typedef Testsuite<TestcaseClass, testcaseStorage> LocalTestsuite;
    typedef TestcaseClass LocalTestcase;
    
    Testsuite(const char * myname, const char * mydesc)
    : TestsuiteBase(myname, mydesc)
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

    virtual void run(Logger& logger) {
      run_tests(getTests(), logger);
    }
  };

  // ------------------------------------------------------------------------

  // Testsuite Auto registration
  // ---------------------------

  typedef std::list<TestsuiteBase*> Testsuites;
  Logger* logger;

  class TestsuiteRegistration {
  public:
    static Testsuites* storage;

    // constructor which in fact registers the testsuite
    static void add(TestsuiteBase* testsuite) {
      if (!storage) storage = new Testsuites();
      storage->push_back(testsuite);
    }

    static int run(Logger& logger) {
      if (!storage) return -1;

      for (Testsuites::iterator i = storage->begin(); 
           i != storage->end(); ++i)
      {
        (*i)->run(logger);  
      }

      return logger.getFailed();
    }
  };

  template <class CurrentTestsuite>
  class RegisterTestsuite {
  public:
    // constructor which in fact registers the testsuite
    RegisterTestsuite() {
      TestsuiteRegistration::add(new CurrentTestsuite());
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

      virtual void test_failed(Testcase& testcase, Result& res)
      {
        std::cout 
          << std::right
          #if ADAPTEST_AUTONAMES
          << std::setw(40)
          << testcase.getDesc()
          #else
          << std::setw(20)
          << testcase.getName()
          #endif
          << " : "
          << res.line
          << " : "
          << std::left
          << std::setw(40)
          << res.msg
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

      virtual void test_error(Testcase& test, Result& res)
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
          << res.line
          << " : "
          << std::left
          << std::setw(40)
          << res.msg
          << std::endl;
        failed_tests++;
        num_tests++;
      }
    };  

  #endif //ADAPTEST_DEFAULT_LOGGER

  // Main entrance Functions
  // -----------------------

  inline
  int run(Logger& logger) {
    return TestsuiteRegistration::run(logger);
  }

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
  class _name;                                                                 \
  ADAPTEST_NAMESPACE::RegisterTestsuite<_name> _name##Reg;                     \
  ADAPTEST_NAMESPACE::Testcases* _name##List = 0;                              \
  typedef ADAPTEST_NAMESPACE::Testsuite<_testcase,_name##List> _name##Base;    \
  class _name : public _name##Base                                             \
  {                                                                            \
  public:                                                                      \
    _name()                                                                    \
    : _name##Base(#_name, _desc)                                               \
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
    std::string name;                                                          \
    std::string desc;                                                          \
    public:                                                                    \
    _name() : name(#_name), desc(_desc) {}                                     \
    virtual std::string& getName() { return name; }                            \
    virtual std::string& getDesc() { return desc; }                            \
    virtual ADAPTEST_NAMESPACE::Result run() {                                 \

#define END_TESTCASE()                                                         \
      return ADAPTEST_NAMESPACE::OK;                                           \
    }                                                                          \
  };                                                                           \

// Perform Tests
// -------------

// call a test function and return upon failure. DRY principle
#define TEST(testtype, ...)  {                                                 \
  const ADAPTEST_NAMESPACE::Result retval =                                    \
    test_##testtype(__VA_ARGS__, __LINE__);                                    \
  if (retval.resval != ADAPTEST_NAMESPACE::OK) return retval;                  \
}

// Adapted Global Variables
// ------------------------

#define ADAPTEST_GLOBALS()                                                     \
  ADAPTEST_NAMESPACE::Testsuites*                                              \
    ADAPTEST_NAMESPACE::TestsuiteRegistration::storage = 0;                    \


// Run Autoregistered Testsuites
// -----------------------------

#define ADAPTEST_MAIN(LoggerClass)                                             \
  ADAPTEST_GLOBALS()                                                           \
  int main(int argc, char const *argv[])                                       \
  {                                                                            \
    ADAPTEST_NAMESPACE::LoggerClass logger;                                    \
    return ADAPTEST_NAMESPACE::run(logger);                                    \
  }                                                                            \

#endif //ADAPTEST_H
