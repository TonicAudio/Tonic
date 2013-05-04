//
//  TonicLoggingTests.m
//  TonicDemo
//
//  Created by Nick D on 4/21/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#import "TonicLoggingTests.h"

#include "Tonic.h"

#define kTestOutputBlockSize kSynthesisBlockSize*4

using namespace Tonic;

@interface TonicLoggingTests ()

{
  TonicFrames testFrames;
  Tonic_::SynthesisContext_ testContext;
  
  float * stereoOutBuffer;
  float * monoOutBuffer;
}

- (void)logTestFrames;

@end

@implementation TonicLoggingTests

- (void)setUp{
  [super setUp];

  stereoOutBuffer = new float[kTestOutputBlockSize * 2];
  monoOutBuffer = new float[kTestOutputBlockSize];
  testFrames.resize(kSynthesisBlockSize, 1, 0);
}

- (void)tearDown{
  [super tearDown];
  
  delete [] stereoOutBuffer;
  delete [] monoOutBuffer;
}

- (void)logTestFrames
{
  for (int i=0; i<testFrames.frames(); i++){
    NSLog(@"[%d]: %.3f", i, testFrames(i, 0));
  }
}

#pragma mark - Tests

- (void)test100ADSRExponential
{
  ControlTrigger trigger;
  ADSR adsr = ADSR(0,0.001,0,0).exponential(true).doesSustain(false).trigger(trigger);

  trigger.trigger();
  adsr.tick(testFrames, testContext);
  
  [self logTestFrames];
}


- (void)test101ADSRLinear
{
  ControlTrigger trigger;
  ADSR adsr = ADSR(0.0005,0.0005,0.001,0).doesSustain(false).trigger(trigger);
  
  trigger.trigger();
  adsr.tick(testFrames, testContext);
  
  [self logTestFrames];
}

@end
