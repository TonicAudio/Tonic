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

// Adaptest Testcase Base Class with test_eq specialised for Floats

#ifndef ADAPTEST_FLOAT_H
#define ADAPTEST_FLOAT_H

#include <adaptest.h>
#include <cmath>

// set testcase default epsilon for comparing float and double
// NOTE: these value are completely arbitrary currently
#ifndef ADAPTEST_DEFAULT_EPSILON_FLOAT
#define ADAPTEST_DEFAULT_EPSILON_FLOAT 0.000000001
#endif

// set testcase default epsilon for comparing float and double
// NOTE: these value are completely arbitrary currently
#ifndef ADAPTEST_DEFAULT_EPSILON_DOUBLE
#define ADAPTEST_DEFAULT_EPSILON_DOUBLE 0.000000000000001
#endif

namespace ADAPTEST_NAMESPACE {

  class FloatingPointTestcase : public virtual Testcase {
  public:
    virtual std::string& getName() = 0;
    virtual std::string& getDesc() = 0;

    float  epsilon_float;
    double epsilon_double;

    FloatingPointTestcase()
    : epsilon_float(ADAPTEST_DEFAULT_EPSILON_FLOAT)
    , epsilon_double(ADAPTEST_DEFAULT_EPSILON_DOUBLE)
    {}

    // make test_eq overridable
    using Testcase::test_eq;

  private:      
    template <class T>
    Result test_diff(
      std::ostream& msg, const int line, 
      T expected, T value, T epsilon, std::string testname) 
    {
      const T diff = std::abs(expected - value);

      if (diff > epsilon)
        return fail(msg, line, expected, value, diff, testname);
      return OK;
    }
  
  public:

    Result test_eq(
      std::ostream& msg, const int line, 
      float expected, float value, std::string testname)
    {
      return test_diff(msg, line, expected, value, epsilon_float,testname);
    }


    Result test_eq(
      std::ostream& msg, const int line, 
      double expected, double value, std::string testname)
    {
      return test_diff(msg, line, expected,value,epsilon_double, testname);
    }

    //--------------------------------------------------------------------------

    template <class A, class B, class C>
    Result fail(
      std::ostream& msg, const int line, 
      A expected, B value, C diff, std::string& testname) 
    {
      msg << testname 
          << " expected to be " << expected 
          << " but is " << value
          << " diff is " << diff
          << " on line " << line;
      return FAILED;
    }   

  };

} // namespace ADAPTEST_NAMESPACE

#endif //ADAPTEST_FLOAT_H