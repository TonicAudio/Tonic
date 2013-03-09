//
//  TonicTests.mm
//  TonicTests
//
//  Created by Nick Donaldson on 3/9/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#import "TonicTests.h"

#include "Tonic.h"
#include "StereoFixedTestGen.h"

using namespace Tonic;

@interface TonicTests ()
{
  TonicFrames testFrames;
  Tonic_::SynthesisContext_ testContext;  
}

- (void)configureStereo:(BOOL)stereo;
- (void)verifyFixedOutputEquals:(float)expectedOutput;

@end

@implementation TonicTests

- (void)setUp{
  [super setUp];
  [self configureStereo:NO];
}

- (void)configureStereo:(BOOL)stereo
{
  testFrames.resize(kSynthesisBlockSize, stereo ? 2 : 1, 0);
}

- (void)verifyFixedOutputEquals:(float)expectedOutput
{
  for (unsigned int i=0; i<testFrames.size(); i++){
    STAssertEquals(testFrames[i], expectedOutput, @"Did not produce expected output");
    if (testFrames[i] != expectedOutput) break;
  }
}

- (void)verifyStereoFixedOutputEqualsLeft:(float)l right:(float)r
{
  for (unsigned int i=0; i<testFrames.frames(); i++){
    STAssertEquals(testFrames[2*i], l, @"Left channel not produce expected output");
    if (testFrames[2*i] != l) break;
    
    STAssertEquals(testFrames[2*i+1], r, @"Right channel did not produce expected output");
    if (testFrames[2*i+1] != r) break;
  }
}

#pragma mark - Generator Tests

- (void)test100FixedValue
{
  // See if FixedValue is correctly filled with a float
  
  [self configureStereo:NO];
  
  float testFloat = 0.533;
  FixedValue testVal = FixedValue(testFloat);
  
  testVal.tick(testFrames, testContext);

  [self verifyFixedOutputEquals:testFloat];
  
}

- (void)test101TickMonoToStereo
{
  // Tick a mono source to a stereo frames object. Ensure both channels equal.
  
  [self configureStereo:YES];
  
  float testFloat = 0.533;
  FixedValue testVal = FixedValue(testFloat);
  
  testVal.tick(testFrames, testContext);
  
  STAssertEquals(testFrames.channels(), (unsigned int)2, @"TestFrames should have 2 channels");
  
  [self verifyFixedOutputEquals:testFloat];
}

- (void)test102TickStereoToMono
{
  // Tick a stereo source to a mono frames object. Ensure mono value is as expected (average of L-R channels)
  
  [self configureStereo:NO];
  
  StereoFixedTestGen stereoVal = StereoFixedTestGen(0.5, 1.0); // 0.5 in left channel, 1.0 in right
  
  STAssertTrue(stereoVal.isStereo(), @"StereoFixedTestGen should output stereo");
  
  stereoVal.tick(testFrames, testContext);
  
  // Result should be average, or 0.75
  [self verifyFixedOutputEquals:0.75f];
}

- (void)test103AdderMonoMono
{
  // Add two mono generators, ensure result is mono
  
  [self configureStereo:NO];
  
  Adder monoAdder = FixedValue(0.5) + FixedValue(0.5);
  
  STAssertFalse(monoAdder.isStereo(), @"Adder should be mono");
  
  monoAdder.tick(testFrames, testContext);
  
  [self verifyFixedOutputEquals:1.0f];
  
}

- (void)test104AdderMonoStereo
{
  // Add one mono, one stereo, ensure result is stereo
  
  [self configureStereo:YES];
  
  Adder stereoAdder = FixedValue(0.5) + StereoFixedTestGen(0.0, 1.0);
  
  STAssertTrue(stereoAdder.isStereo(), @"Adder should be stereo");
  
  stereoAdder.tick(testFrames, testContext);
  
  [self verifyStereoFixedOutputEqualsLeft:0.5f right:1.5f];
  
}

- (void)test105AdderStereoStereo
{
  [self configureStereo:YES];
  
  Adder stereoAdder = StereoFixedTestGen(1.0, 0.0) + StereoFixedTestGen(0.0, 1.0);
  
  STAssertTrue(stereoAdder.isStereo(), @"Adder should be stereo");
  
  stereoAdder.tick(testFrames, testContext);
  
  [self verifyFixedOutputEquals:1.0f];
  
}

- (void)test106MultMonoMono
{
  // Multiply two mono generators, ensure result is mono
  
  [self configureStereo:NO];
  
  Multiplier monoMult = FixedValue(0.5) * FixedValue(0.5);
  
  STAssertFalse(monoMult.isStereo(), @"Multiplier should be mono");
  
  monoMult.tick(testFrames, testContext);
  
  [self verifyFixedOutputEquals:0.25f];
  
}

- (void)test107MultMonoStereo
{
  // Multiply one mono, one stereo, ensure result is stereo
  
  [self configureStereo:YES];
  
  Multiplier stereoMult = FixedValue(0.5) * StereoFixedTestGen(0.0, 1.0);
  
  STAssertTrue(stereoMult.isStereo(), @"Multiplier should be stereo");
  
  stereoMult.tick(testFrames, testContext);
  
  [self verifyStereoFixedOutputEqualsLeft:0.0f right:0.5f];
  
}

- (void)test108MultStereoStereo
{
  [self configureStereo:YES];
  
  Multiplier stereoMult = StereoFixedTestGen(1.0, 0.0) * StereoFixedTestGen(0.0, 1.0);
  
  STAssertTrue(stereoMult.isStereo(), @"Multiplier should be stereo");
  
  stereoMult.tick(testFrames, testContext);
  
  // Both channels 0
  [self verifyFixedOutputEquals:0.0f];
  
}
#pragma mark - Control Generator Tests

// TODO

@end
