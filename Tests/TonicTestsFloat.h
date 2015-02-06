// Tonictest Testcase with test_eq specialised for Floats

#ifndef TONICTESTSFLOAT_H
#define TONICTESTSFLOAT_H

#include "TonicTests.h"
#include <cmath>

// set testcase default epsilon for comparing float and double
// NOTE: these value are completely arbitrary currently
#ifndef TONICTESTS_DEFAULT_EPSILON_FLOAT
#define TONICTESTS_DEFAULT_EPSILON_FLOAT 0.000000001
#endif

// set testcase default epsilon for comparing float and double
// NOTE: these value are completely arbitrary currently
#ifndef TONICTESTS_DEFAULT_EPSILON_DOUBLE
#define TONICTESTS_DEFAULT_EPSILON_DOUBLE 0.000000000000001
#endif

namespace TONICTESTS_NAMESPACE {

	class FloatingPointTestcase : public Testcase {
	public:
		virtual std::string& getName() = 0;
		virtual std::string& getDesc() = 0;

		float  epsilon_float;
		double epsilon_double;

		FloatingPointTestcase()
		: epsilon_float(TONICTESTS_DEFAULT_EPSILON_FLOAT)
		, epsilon_double(TONICTESTS_DEFAULT_EPSILON_DOUBLE)
		{}

		// make test_eq overridable
		using Testcase::test_eq;

	private:			
		template <class T>
		Result float_test(
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
			return float_test(failmsg, line, expected, value, epsilon_float,testname);
		}


		Result test_eq(
			std::ostream& failmsg, const int line, 
			double expected, double value, std::string testname)
		{
			return float_test(failmsg, line, expected,value,epsilon_double, testname);
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

} // namespace TONICTESTS_NAMESPACE

#endif //TONICTESTSFLOAT_H