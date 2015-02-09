#include "TonicTests.h"

//
//  TonicTests.mm
//  TonicTests
//
//  Created by Nick Donaldson on 3/9/13.

//

#include "TonicTests.h"
#include "TonicTestsFloat.h"

#include "Tonic.h"
#include "TestBufferFiller.h"
#include "StereoFixedTestGen.h"

using namespace TonicTests;

#define kTestOutputBlockSize kSynthesisBlockSize*4

using namespace Tonic;

  class TestControlChangeSubscriber : public ControlChangeSubscriber{
    public:
    bool valueChangedFlag;
    TestControlChangeSubscriber() : valueChangedFlag(false){}
    void valueChanged(string, TonicFloat){
      valueChangedFlag = true;
    }
  };

// ================================================================

namespace Tonic { 
  namespace Tonic_{
  
    StereoFixedTestGen_::StereoFixedTestGen_(){
      
    }
    
    StereoFixedTestGen_::~StereoFixedTestGen_(){
      
    }
  
  } // Namespace Tonic_
} // Namespace Tonic

// ======================================================

class TonicTestcase : public FloatingPointTestcase {
public:

  TonicFrames testFrames;
  Tonic_::SynthesisContext_ testContext;
  
  float * stereoOutBuffer;
  float * monoOutBuffer;

  virtual void setUp(){

    // TODO: [self configureStereo:NO];
    
    stereoOutBuffer = new float[kTestOutputBlockSize * 2];
    monoOutBuffer = new float[kTestOutputBlockSize];
  }

  virtual void tearDown(){
    delete [] stereoOutBuffer;
    delete [] monoOutBuffer;
  }

  void configureStereo(bool stereo){
    testFrames.resize(kSynthesisBlockSize, stereo ? 2 : 1, 0);
  }

  // ----------------------------------------------------------------

    // allow overriding of test_eq with buffers
    using FloatingPointTestcase::test_eq;

    Result test_fixedOutputEquals(
      std::ostream& failmsg, const int line, float expectedOutput, std::string name) 
    {
      for (unsigned int i=0; i<testFrames.size(); i++){
        TEST(eq, expectedOutput, testFrames[i],
                   "%s: Did not produce expected output at index %i" /*, name.c_str(), i*/ );
      }

      return OK;
    }

  // ----------------------------------------------------------------

    Result test_stereoFixedOutputEquals( 
      std::ostream& failmsg, const int line, TonicFrames& frames, float l, float r, std::string name) 
    {
      for (unsigned int i=0; i<frames.frames(); i++){
        TEST(eq, l, frames[2*i],   Formatted("Left channel, frame %i", i))
        TEST(eq, r, frames[2*i+1], Formatted("Right channel, frame %i", i))
      }

      return OK;
    }

  // ----------------------------------------------------------------

    Result test_bufferFillerMonoFixedOutputEquals( 
      std::ostream& failmsg, const int line, float expectedOutput, std::string name)
    {
      for (unsigned int i=0; i<kTestOutputBlockSize; i++){
        TEST(eq, expectedOutput, monoOutBuffer[i], 
                   "%s: Did not produce expected output at index %i" /*, name.c_str(), i*/ );
      }

      return OK;
    }

  // ----------------------------------------------------------------

    Result test_bufferFillerStereoFixedOutputEquals( 
      std::ostream& failmsg, const int line, float l, float r, std::string name)
    {
      for (unsigned int i=0; i<kTestOutputBlockSize; i++)
      {
        TEST(eq, l, stereoOutBuffer[2*i], 
           "%s: Left channel not produce expected output on frame %i" /*, name.c_str(), i*/ );

        TEST(eq, r, stereoOutBuffer[2*i+1], 
          "%s: Right channel did not produce expected output on frame %i" /*, name.c_str(), i*/ );
      }

      return OK;
    }

};

// ================================================================

// The Tests
// ---------

TESTSUITE(MyTests, TonicTestcase, "a simple Testsuite")

  // a simple toggle for me ^^ 
  //#define DEMO_TESTCASE
  
  #ifdef DEMO_TESTCASE
  TESTCASE("Demo")
    int i = 0;
    TEST(eq, 0, i, "i")
    TEST(eq, true, true, "true")
    TEST(eq, false, false, "false")
    TEST(eq, 0.1f, 0.1f, "0.1f")
  END_TESTCASE()

  #else

  #pragma mark - Generator Tests
  #include "generator.tests"

  #endif
  // #pragma mark - Control Generator Tests
  //#include "controlgenerator.tests"
  
  // #pragma mark - Buffer filler tests
  // #include "bufferfiller.tests"

  // #pragma mark operator tests
  // #include "operator.tests"

END_TESTSUITE()


int main(int argc, char const *argv[])
{
  ConsoleLogger logger;
  MyTests suite(logger);
  suite.run();
  return 0;
}
