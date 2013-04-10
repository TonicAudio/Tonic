//
//  Tests.h
//  TonicDemo
//
//  Created by Morgan Packard on 2/12/13.

//

#ifndef TonicDemo_Tests_h
#define TonicDemo_Tests_h

#include "Synth.h"
#include "RampedValue.h"
#include<time.h> 

namespace Tonic {

  namespace PerformanceTest{
    
    const int NUM_TEST_BUFFERS_TO_FILL = 10000;
  
    void testRampedValue(){
  
      //////// test ramped value ////////
    
      RampedValue testRamp;
      
      TonicFrames testFrames(kSynthesisBlockSize, 1);
    
      clock_t startTime = clock();
    
      for(int i = 0; i < NUM_TEST_BUFFERS_TO_FILL; i++){
          testRamp.tick(testFrames);        
          testRamp.setValue(rand() % 100);
          testRamp.setTarget(rand() % 100);
      }
      
      clock_t endTime = clock();
      float diff = (((float)endTime - (float)startTime) / CLOCKS_PER_SEC ) * 1000;   
      
      printf("[Tonic] Tested RampedValue. Time to fill %i TonicFrames: %f\n",NUM_TEST_BUFFERS_TO_FILL, diff);

    }
  }

  void runPerformanceTests(){
  
    
    printf("\n\n[Tonic] Running performance tests.\n ");
    
    PerformanceTest::testRampedValue();
    
  }
}

#endif
