// Tonictest Testcase with test_eq specialised for Floats

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

	class FloatingPointTestcase : public Testcase {
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
			std::ostream& failmsg, const int line, 
			T expected, T value, T epsilon, std::string testname) 
		{
			const T diff = std::abs(expected - value);

			if (diff > epsilon)
				return write_fail(failmsg, line, expected, value, diff, testname);
			return OK;
		}
	
	public:

		Result test_eq(
			std::ostream& failmsg, const int line, 
			float expected, float value, std::string testname)
		{
			return test_diff(failmsg, line, expected, value, epsilon_float,testname);
		}


		Result test_eq(
			std::ostream& failmsg, const int line, 
			double expected, double value, std::string testname)
		{
			return test_diff(failmsg, line, expected,value,epsilon_double, testname);
		}

		//--------------------------------------------------------------------------

		template <class A, class B, class C>
		Result write_fail(
			std::ostream& failmsg, const int line, 
			A expected, B value, C diff, std::string& testname) 
		{
			failmsg << testname 
							<< " expected to be " << expected 
							<< " but is " << value
							<< " diff is " << diff
							<< " on line " << line;
			return FAILED;
		}		

	};

} // namespace ADAPTEST_NAMESPACE

#endif //ADAPTEST_FLOAT_H