//
//  TonicTests.mm
//  TonicTests
//
//  Created by Nick Donaldson on 3/9/13.
//  adapted by Andreas Koerner 2015-02-10
//

#include "tonictests.h"

TESTSUITE(OperatorTests, TonicTestcase, "")

  TESTCASE(test400CombineGeneratorControlGenerator, "")

    {

      TestBufferFiller testFiller;
      Generator gen =  ControlValue(1) + FixedValue(1);
      testFiller.setOutputGen(gen);
      testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
      TEST(eq, 2.f, *stereoOutBuffer, "ControlValue(1) + FixedValue(1) failed");
    
    }
    
    {

      TestBufferFiller testFiller;
      Generator gen =   FixedValue(1) + ControlValue(1);
      testFiller.setOutputGen(gen);
      testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
      TEST(eq, 2.f, *stereoOutBuffer, "FixedValue(1) + ControlValue(1) failed");
    
    }
    
    {
    
      TestBufferFiller testFiller;
      Generator gen =   FixedValue(3) - ControlValue(2);
      testFiller.setOutputGen(gen);
      testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
      TEST(eq, 1.f, *stereoOutBuffer, "FixedValue(1) + ControlValue(1) failed");

    }
    
    {
    
      TestBufferFiller testFiller;
      Generator gen =   ControlValue(3) - FixedValue(2);
      testFiller.setOutputGen(gen);
      testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
      TEST(eq, 1.f, *stereoOutBuffer, "ControlValue(3) - FixedValue(2) failed");
    }
    
    {
    
      TestBufferFiller testFiller;
      Generator gen =   ControlValue(3) * FixedValue(2);
      testFiller.setOutputGen(gen);
      testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
      TEST(eq, 6.f, *stereoOutBuffer, "ControlValue(3) * FixedValue(2) failed");
    }
    
    {
    
      TestBufferFiller testFiller;
      Generator gen =  FixedValue(3) * ControlValue(2);
      testFiller.setOutputGen(gen);
      testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
      TEST(eq, 6.f, *stereoOutBuffer, " FixedValue(3) * ControlValue(2) failed");
    }
    
  END_TESTCASE()

  TESTCASE(test401GeneratorMinusControlGenerator, "")

    {

    TestBufferFiller testFiller;
    Generator gen =  ControlValue(2) - FixedValue(1);
    testFiller.setOutputGen(gen);
    testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
    TEST(eq, 1.f, *stereoOutBuffer, "ControlValue(2) - FixedValue(1) failed");
    
    }
    
    {

    TestBufferFiller testFiller;
    Generator gen =   FixedValue(2) - ControlValue(1);
    testFiller.setOutputGen(gen);
    testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
    TEST(eq, 1.f, *stereoOutBuffer, "FixedValue(1) - ControlValue(1) failed");
    
    }
    
  
  END_TESTCASE()

  TESTCASE(test402ControlGeneratorDivide, "")
    {
      ControlGenerator gen = ControlValue(10) / ControlValue(5);
      Tonic_::SynthesisContext_ context;
      TEST(eq, gen.tick(context).value, 2.f, "ControlValue(10) / ControlValue(5) failed.");
    }

    {
      ControlGenerator gen = 10 / ControlValue(5);
      Tonic_::SynthesisContext_ context;
      TEST(eq, gen.tick(context).value, 2.f, "10 / ControlValue(5) failed.");
    }

    {
      ControlGenerator gen = ControlValue(10) / 5;
      Tonic_::SynthesisContext_ context;
      TEST(eq, gen.tick(context).value, 2.f, "ControlValue(10) / 5 failed.");
    }

  
  END_TESTCASE()

  TESTCASE(test403ControlGenDivideByZero, "")
    ControlValue right = ControlValue(5);
    ControlGenerator gen = ControlValue(10) / right;
    Tonic_::SynthesisContext_ context;
    gen.tick(context);
    right.value(0);
    
    TEST(eq, gen.tick(context).value, 2.f, "Divide by zero should return the last valid value.");
  
  END_TESTCASE()


  TESTCASE(test404TestCombinationsOfGenAndControlGen, "")

    TestBufferFiller testFiller;
    ControlGenerator ctrlGen1 = ControlValue(2);
    Generator gen1 = ctrlGen1 + FixedValue(2) * ctrlGen1;
    testFiller.setLimitOutput(false);
    testFiller.setOutputGen(gen1);
    testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
    TEST(eq, *stereoOutBuffer, 6.f, "Complex combination of control gen and gen failed");
     
    // set the force output flag and try it again to ensure it still works
    testFiller.forceNewOutput();
    testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
    TEST(eq, *stereoOutBuffer, 6.f, "Complex combination of control gen and gen failed");
    
  
  END_TESTCASE()

  TESTCASE(test405ControlGeneratorComparison, "")
    
    const int n_iterations = 10;
    Tonic_::SynthesisContext_ context;
    
    // equals
    for (int i=0; i<n_iterations; i++){
      
      float rf = randomFloat(-1000.f, 1000.f);
      
      ControlGenerator g = ControlValue(rf) == ControlValue(rf);
      ControlGenerator g1 = ControlValue(rf) == rf;
      ControlGenerator g2 = ControlValue(rf) == ControlValue(99999.f);
      
      TEST(true, g.tick(context).triggered, "Every tick should produce a change");
      
      TEST(eq, g.tick(context).value, 1.0f, Formatted("%.2f should equal itself", rf));
      TEST(eq, g1.tick(context).value, 1.0f, Formatted("%.2f should equal itself", rf));
      TEST(eq, g2.tick(context).value, 0.0f, Formatted("%.2f should not be equal to rhs", rf));

    }
    
    // not equals
    for (int i=0; i<n_iterations; i++){
      
      float rf = randomFloat(-1000.f, 1000.f);
      
      ControlGenerator g = ControlValue(rf) != ControlValue(99999.f);
      ControlGenerator g1 = ControlValue(rf) != 99999.f;
      ControlGenerator g2 = ControlValue(rf) != ControlValue(rf);
      
      TEST(true, g.tick(context).triggered, "Every tick should produce a change");
      
      TEST(eq, g.tick(context).value, 1.0f, Formatted("%.2f should not be equal to rhs", rf));
      TEST(eq, g1.tick(context).value, 1.0f, Formatted("%.2f should not be equal to rhs", rf));
      TEST(eq, g2.tick(context).value, 0.0f, Formatted("%.2f should be equal to rhs", rf));
      
    }
    
    // greater than
    for (int i=0; i<n_iterations; i++){
      
      float rf = randomFloat(0.1f, 1000.f);
      
      ControlGenerator g = ControlValue(rf) > ControlValue(0.f);
      ControlGenerator g1 = ControlValue(rf) > 0.f;
      ControlGenerator g2 = ControlValue(rf) > ControlValue(99999.f);
      
      TEST(true, g.tick(context).triggered, "Every tick should produce a change");
      
      TEST(eq, g.tick(context).value, 1.0f, Formatted("%.2f should be greater than rhs", rf));
      TEST(eq, g1.tick(context).value, 1.0f, Formatted("%.2f should be greater than rhs", rf));
      TEST(eq, g2.tick(context).value, 0.0f, Formatted("%.2f should not be greater than rhs", rf));
      
    }
    
    // greater than or equal
    for (int i=0; i<n_iterations; i++){
      
      float rf = randomFloat(0.1f, 1000.f);
      
      ControlGenerator g = ControlValue(rf) >= ControlValue(rf);
      ControlGenerator g1 = ControlValue(rf) > 0.f;
      ControlGenerator g2 = ControlValue(rf) >= ControlValue(99999.f);
      
      TEST(true, g.tick(context).triggered, "Every tick should produce a change");
      
      TEST(eq, g.tick(context).value, 1.0f, Formatted("%.2f should be greater than or equal to rhs", rf));
      TEST(eq, g1.tick(context).value, 1.0f, Formatted("%.2f should be greater than or equal to rhs", rf));
      TEST(eq, g2.tick(context).value, 0.0f, Formatted("%.2f should not be greater than or equal rhs", rf));
      
    }
    
    // less than
    for (int i=0; i<n_iterations; i++){
      
      float rf = randomFloat(0.1f, 1000.f);
      
      ControlGenerator g = ControlValue(rf) < ControlValue(99999.f);
      ControlGenerator g1 = ControlValue(rf) < 99999.f;
      ControlGenerator g2 = ControlValue(rf) < ControlValue(0.f);
      
      TEST(true, g.tick(context).triggered, "Every tick should produce a change");
      
      TEST(eq, g.tick(context).value, 1.0f, Formatted("%.2f should be less than rhs", rf));
      TEST(eq, g1.tick(context).value, 1.0f, Formatted("%.2f should be less than rhs", rf));
      TEST(eq, g2.tick(context).value, 0.0f, Formatted("%.2f should not be less than rhs", rf));
      
    }
    
    // less than or equal
    for (int i=0; i<n_iterations; i++) {
      
      float rf = randomFloat(0.1f, 1000.f);
      
      ControlGenerator g = ControlValue(rf) <= ControlValue(rf);
      ControlGenerator g1 = ControlValue(rf) <= 99999.f;
      ControlGenerator g2 = ControlValue(rf) <= ControlValue(0.f);
      
      TEST(true, g.tick(context).triggered, "Every tick should produce a change");
      
      TEST(eq, g.tick(context).value, 1.0f, Formatted("%.2f should be less than or equal to rhs", rf));
      TEST(eq, g1.tick(context).value, 1.0f, Formatted("%.2f should be less than or equal to rhs", rf));
      TEST(eq, g2.tick(context).value, 0.0f, Formatted("%.2f should not be less than or equal to rhs", rf));
      
    }
  
  END_TESTCASE()

END_TESTSUITE()


int main(int argc, char const *argv[])
{
  ConsoleLogger logger;
  OperatorTests suite(logger);
  suite.run();
  return logger.getFailed();
}
