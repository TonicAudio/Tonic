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
#define ADAPTEST_BUFWRITE_FILE 1

#include <adaptest.h>
#include <adaptest/float.h>
#include <adaptest/buf.h>

#include "Tonic.h"
#include "TestBufferFiller.h"
#include "StereoFixedTestGen.h"

#include <iostream>

using std::cin;
using std::cout;
using namespace TonicTests;
using namespace Tonic;

#define TONICTEST_MAIN()                                                       \
  ADAPTEST_GLOBALS()                                                           \
  int main(int argc, char const *argv[])                                       \
  {                                                                            \
    int res;                                                                   \
    {                                                                          \
      ADAPTEST_NAMESPACE::ConsoleLogger logger;                                \
      res = ADAPTEST_NAMESPACE::run(logger);                                   \
    }                                                                          \
    cout << "Press Return to exit" << std::endl;                               \
    cin.get();                                                                 \
    return res;                                                                \
  }                                                                            \


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

class TonicTestcase : public FloatingPointTestcase, 
                      public BufferTestcase<CSVBufferWriter> {
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
      TonicFrames& frames, float expectedOutput, const int line) 
    {
      for (unsigned int i=0; i<frames.size(); i++){
        TEST(eq, expectedOutput, frames[i], format("frame {}", i))
      }

      return OK;
    }

  // ----------------------------------------------------------------

    Result test_stereoFixedOutputEquals( 
      TonicFrames& frames, float l, float r, const int line) 
    {
      for (unsigned int i=0; i<frames.frames(); i++){
        TEST(eq, l, frames[2*i],   format("Left channel, frame {}", i))
        TEST(eq, r, frames[2*i+1], format("Right channel, frame {}", i))
      }

      return OK;
    }

  // ----------------------------------------------------------------

    Result test_bufferFillerMonoFixedOutputEquals( 
      float expectedOutput, const int line)
    {
      for (unsigned int i=0; i<kTestOutputBlockSize; i++){
        TEST(eq, expectedOutput, monoOutBuffer[i], 
                   "%s: Did not produce expected output at index %i" /*, name.c_str(), i*/ );
      }

      return OK;
    }

  // ----------------------------------------------------------------

    Result test_bufferFillerStereoFixedOutputEquals( 
      float l, float r, const int line)
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