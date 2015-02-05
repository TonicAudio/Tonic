#include "TonicTests.h"

//
//  TonicTests.mm
//  TonicTests
//
//  Created by Nick Donaldson on 3/9/13.

//

#include "TonicTests.h"

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

class TonicTestcase : public Testcase {
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

    Result test_FixedOutputEquals(
      float expectedOutput, std::string name, std::ostream& failmsg) 
    {
      for (unsigned int i=0; i<testFrames.size(); i++){
        TEST(eq, expectedOutput, testFrames[i],
                   "%s: Did not produce expected output at index %i" /*, name.c_str(), i*/ );
      }

      return OK;
    }

  // ----------------------------------------------------------------

    Result test_StereoFixedOutputEquals( 
      float l, float r, std::string name, std::ostream& failmsg) 
    {
      for (unsigned int i=0; i<testFrames.frames(); i++){
        TEST(eq, l, testFrames[2*i], 
           "%s: Left channel not produce expected output on frame %i" /*, name.c_str(), i*/ );

        TEST(eq, r, testFrames[2*i+1], 
          "%s: Right channel did not produce expected output on frame %i" /*, name.c_str(), i*/ );
      }

      return OK;
    }

  // ----------------------------------------------------------------

    Result test_BufferFillerMonoFixedOutputEquals( 
      float expectedOutput, std::string name, std::ostream& failmsg)
    {
      for (unsigned int i=0; i<kTestOutputBlockSize; i++){
        TEST(eq, expectedOutput, monoOutBuffer[i], 
                   "%s: Did not produce expected output at index %i" /*, name.c_str(), i*/ );
      }

      return OK;
    }

  // ----------------------------------------------------------------

    Result test_BufferFillerStereoFixedOutputEquals( 
      float l, float r, std::string name, std::ostream& failmsg)
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

TonicTests::Testcases* MyTests_Storage = 0;
class MyTests : public TonicTests::Testsuite<TonicTestcase, &MyTests_Storage>
{
public:
  MyTests(Logger& mylogger)
  : TonicTests::Testsuite<TonicTestcase, &MyTests_Storage>(mylogger, "MySuite")
  {}
private:

  TESTCASE("Demo")
    int i = 0;
    TEST(eq, 1, i, "i")
  END_TESTCASE()

  #pragma mark - Generator Tests
  //#include "generator.tests"

  // #pragma mark - Control Generator Tests
  //#include "controlgenerator.tests"
  
  // #pragma mark - Buffer filler tests
  // #include "bufferfiller.tests"

  // #pragma mark operator tests
  // #include "operator.tests"

}; // namespace TonicTests

int main(int argc, char const *argv[])
{
  ConsoleLogger logger;
  MyTests suite(logger);
  suite.run();
  return 0;
}