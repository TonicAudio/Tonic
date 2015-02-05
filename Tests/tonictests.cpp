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

class TonicTestcase : public TestcaseBase {
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

    Result testFixedOutputEquals(
      float expectedOutput, std::string name, std::ostream& failmsg) 
    {
      for (unsigned int i=0; i<testFrames.size(); i++){
        TEST_EQUAL(expectedOutput, testFrames[i],
                   "%s: Did not produce expected output at index %i" /*, name.c_str(), i*/ );
      }

      return OK;
    }

  // ----------------------------------------------------------------

    Result testStereoFixedOutputEquals( 
      float l, float r, std::string name, std::ostream& failmsg) 
    {
      for (unsigned int i=0; i<testFrames.frames(); i++){
        TEST_EQUAL(l, testFrames[2*i], 
           "%s: Left channel not produce expected output on frame %i" /*, name.c_str(), i*/ );

        TEST_EQUAL(r, testFrames[2*i+1], 
          "%s: Right channel did not produce expected output on frame %i" /*, name.c_str(), i*/ );
      }

      return OK;
    }

  // ----------------------------------------------------------------

    Result testBufferFillerMonoFixedOutputEquals( 
      float expectedOutput, std::string name, std::ostream& failmsg)
    {
      for (unsigned int i=0; i<kTestOutputBlockSize; i++){
        TEST_EQUAL(expectedOutput, monoOutBuffer[i], 
                   "%s: Did not produce expected output at index %i" /*, name.c_str(), i*/ );
      }

      return OK;
    }

  // ----------------------------------------------------------------

    Result testBufferFillerStereoFixedOutputEquals( 
      float l, float r, std::string name, std::ostream& failmsg)
    {
      for (unsigned int i=0; i<kTestOutputBlockSize; i++)
      {
        TEST_EQUAL(l, stereoOutBuffer[2*i], 
           "%s: Left channel not produce expected output on frame %i" /*, name.c_str(), i*/ );

        TEST_EQUAL(r, stereoOutBuffer[2*i+1], 
          "%s: Right channel did not produce expected output on frame %i" /*, name.c_str(), i*/ );
      }

      return OK;
    }

};

template <>
TonicTests::Testsuite<TonicTestcase>::Testcases* 
  TonicTests::Testsuite<TonicTestcase>::s_tests = 0;

// ================================================================

// The Tests
// ---------

class MyTests : public TonicTests::Testsuite<TonicTestcase>
{

  #pragma mark - Generator Tests
  #include "generator.tests"

  // #pragma mark - Control Generator Tests
  //#include "controlgenerator.tests"
  
  // #pragma mark - Buffer filler tests
  // #include "bufferfiller.tests"

  // #pragma mark operator tests
  // #include "operator.tests"

}; // namespace TonicTests

int main(int argc, char const *argv[])
{
  MyTests suite;
  suite.run();
  return 0;
}