//
//  AdapTest.h
//  AdapTest
//
//  Created by Andreas Koerner 2015-02-06
//  Copyright (c) 2015 Andreas Koerner. All rights reserved.
//

#ifndef TONICTESTS_H
#define TONICTESTS_H

#define ADAPTEST_NAMESPACE TonicTests

#include <adaptest.h>
#include <adaptest/float.h>

#include "Tonic.h"
#include "TestBufferFiller.h"
#include "StereoFixedTestGen.h"

using namespace TonicTests;
using namespace Tonic;

#define kTestOutputBlockSize kSynthesisBlockSize*4

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
      std::ostream& failmsg, const int line, 
      TonicFrames& frames, float expectedOutput) 
    {
      for (unsigned int i=0; i<frames.size(); i++){
        TEST(eq, expectedOutput, frames[i], Formatted("frame %i", i))
      }

      return OK;
    }

  // ----------------------------------------------------------------

    Result test_stereoFixedOutputEquals( 
      std::ostream& failmsg, const int line, 
      TonicFrames& frames, float l, float r) 
    {
      for (unsigned int i=0; i<frames.frames(); i++){
        TEST(eq, l, frames[2*i],   Formatted("Left channel, frame %i", i))
        TEST(eq, r, frames[2*i+1], Formatted("Right channel, frame %i", i))
      }

      return OK;
    }

  // ----------------------------------------------------------------

    Result test_bufferFillerMonoFixedOutputEquals( 
      std::ostream& failmsg, const int line, 
      float expectedOutput)
    {
      for (unsigned int i=0; i<kTestOutputBlockSize; i++){
        TEST(eq, expectedOutput, monoOutBuffer[i], 
                   "%s: Did not produce expected output at index %i" /*, name.c_str(), i*/ );
      }

      return OK;
    }

  // ----------------------------------------------------------------

    Result test_bufferFillerStereoFixedOutputEquals( 
      std::ostream& failmsg, const int line, 
      float l, float r)
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


#endif //TONICTESTS_H