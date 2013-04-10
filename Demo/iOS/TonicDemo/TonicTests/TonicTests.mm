//
//  TonicTests.mm
//  TonicTests
//
//  Created by Nick Donaldson on 3/9/13.

//

#import "TonicTests.h"

#include "Tonic.h"
#include "StereoFixedTestGen.h"
#include "ADSR.h"
#include "ControlMetro.h"
#include "ControlStepper.h"
#include "ControlTrigger.h"

#define kTestOutputBlockSize kSynthesisBlockSize*4

using namespace Tonic;

// BufferFiller with publicly settable output generator for testing

class TestBufferFiller : public BufferFiller
{
  
public:
  void setOutputGen(Generator & gen) { outputGen = gen; };
};

// ======================================================

@interface TonicTests ()
{
  TonicFrames testFrames;
  Tonic_::SynthesisContext_ testContext;
  
  float * stereoOutBuffer;
  float * monoOutBuffer;
}

- (void)configureStereo:(BOOL)stereo;
- (void)verifyFixedOutputEquals:(float)expectedOutput;

@end

@implementation TonicTests

- (void)setUp{
  [super setUp];
  [self configureStereo:NO];
  
  stereoOutBuffer = new float[kTestOutputBlockSize * 2];
  monoOutBuffer = new float[kTestOutputBlockSize];
}

- (void)tearDown{
  [super tearDown];
  
  delete [] stereoOutBuffer;
  delete [] monoOutBuffer;
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
    STAssertEquals(testFrames[2*i], l, @"Left channel not produce expected output on frame %i", i);
    if (testFrames[2*i] != l) break;
    
    STAssertEquals(testFrames[2*i+1], r, @"Right channel did not produce expected output on frame %i", i);
    if (testFrames[2*i+1] != r) break;
  }
}

- (void)verifyBufferFillerMonoFixedOutputEquals:(float)expectedOutput
{
  for (unsigned int i=0; i<kTestOutputBlockSize; i++){
    STAssertEquals(monoOutBuffer[i], expectedOutput, @"Did not produce expected output");
    if (monoOutBuffer[i] != expectedOutput) break;
  }
}

- (void)verifyBufferFillerStereoFixedOutputEqualsLeft:(float)l right:(float)r
{
  for (unsigned int i=0; i<kTestOutputBlockSize; i++){
    STAssertEquals(stereoOutBuffer[2*i], l, @"Left channel not produce expected output");
    if (stereoOutBuffer[2*i] != l) break;
    
    STAssertEquals(stereoOutBuffer[2*i+1], r, @"Right channel did not produce expected output");
    if (stereoOutBuffer[2*i+1] != r) break;
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
  
  STAssertTrue(stereoVal.isStereoOutput(), @"StereoFixedTestGen should output stereo");
  
  stereoVal.tick(testFrames, testContext);
  
  // Result should be average, or 0.75
  [self verifyFixedOutputEquals:0.75f];
}

- (void)test103AdderMonoMono
{
  // Add two mono generators, ensure result is mono
  
  [self configureStereo:NO];
  
  Adder monoAdder = FixedValue(0.5) + FixedValue(0.5);
  
  STAssertFalse(monoAdder.isStereoOutput(), @"Adder should be mono");
  
  monoAdder.tick(testFrames, testContext);
  
  [self verifyFixedOutputEquals:1.0f];
  
}

- (void)test104AdderMonoStereo
{
  // Add one mono, one stereo, ensure result is stereo
  
  [self configureStereo:YES];
  
  Adder stereoAdder = FixedValue(0.5) + StereoFixedTestGen(0.0, 1.0);
  
  STAssertTrue(stereoAdder.isStereoOutput(), @"Adder should be stereo");
  
  stereoAdder.tick(testFrames, testContext);
  
  [self verifyStereoFixedOutputEqualsLeft:0.5f right:1.5f];
  
}

- (void)test105AdderStereoStereo
{
  [self configureStereo:YES];
  
  Adder stereoAdder = StereoFixedTestGen(1.0, 0.0) + StereoFixedTestGen(0.0, 1.0);
  
  STAssertTrue(stereoAdder.isStereoOutput(), @"Adder should be stereo");
  
  stereoAdder.tick(testFrames, testContext);
  
  [self verifyFixedOutputEquals:1.0f];
  
}

- (void)test106MultMonoMono
{
  // Multiply two mono generators, ensure result is mono
  
  [self configureStereo:NO];
  
  Multiplier monoMult = FixedValue(0.5) * FixedValue(0.5);
  
  STAssertFalse(monoMult.isStereoOutput(), @"Multiplier should be mono");
  
  monoMult.tick(testFrames, testContext);
  
  [self verifyFixedOutputEquals:0.25f];
  
}

- (void)test107MultMonoStereo
{
  // Multiply one mono, one stereo, ensure result is stereo
  
  [self configureStereo:YES];
  
  Multiplier stereoMult = FixedValue(0.5) * StereoFixedTestGen(0.0, 1.0);
  
  STAssertTrue(stereoMult.isStereoOutput(), @"Multiplier should be stereo");
  
  stereoMult.tick(testFrames, testContext);
  
  [self verifyStereoFixedOutputEqualsLeft:0.0f right:0.5f];
  
}

- (void)test108MultStereoStereo
{
  [self configureStereo:YES];
  
  Multiplier stereoMult = StereoFixedTestGen(1.0, 0.0) * StereoFixedTestGen(0.0, 1.0);
  
  STAssertTrue(stereoMult.isStereoOutput(), @"Multiplier should be stereo");
  
  stereoMult.tick(testFrames, testContext);
  
  // Both channels 0
  [self verifyFixedOutputEquals:0.0f];
  
}

- (void)test109PannerMonoToStereo{
  
  [self configureStereo:YES];
  
  MonoToStereoPanner panner = MonoToStereoPanner().pan(-1).input(FixedValue(1));
  panner.tick(testFrames, testContext);
  [self verifyStereoFixedOutputEqualsLeft:1 right:0];
  
  panner.pan(0);
  panner.tick(testFrames, testContext);
  [self verifyStereoFixedOutputEqualsLeft:1 right:1];
  
  panner.pan(1);
  panner.tick(testFrames, testContext);
  [self verifyStereoFixedOutputEqualsLeft:0 right:1];
  
}

#pragma mark - Control Generator Tests

- (void)test200ControlStepper
{
  // there's a bug involving ControlStepper where it appears to output zeros when it shares an metro with another controlgenerator
  
  {
    ControlTrigger trig;
    ControlStepper stepper;
    stepper = ControlStepper().start(1).end(2).step(1).trigger(trig);
    Tonic_::SynthesisContext_ context;
    ControlGeneratorOutput result = stepper.tick(context);
    STAssertEquals(result.value, 1.0f, @"ControlStepper did not produce expected output");
  }
  
    {
    //setup
    ControlTrigger trig;
    ControlStepper stepper;
    stepper = ControlStepper().start(1).end(3).step(1).trigger(trig);

    Tonic_::SynthesisContext_ context;
    ControlGeneratorOutput result = stepper.tick(context);
    STAssertEquals(result.value, 1.0f, @"ControlStepper did not produce expected output");
    
    trig.trigger();
    context.tick();
    result = stepper.tick(context);
    STAssertEquals(result.value, 2.0f, @"ControlStepper did not produce expected output");
    
    trig.trigger();
    context.tick();
    result = stepper.tick(context);
    STAssertEquals(result.value, 3.0f, @"ControlStepper did not produce expected output");
    
    trig.trigger();
    context.tick();
    result = stepper.tick(context);
    STAssertEquals(result.value, 2.0f, @"ControlStepper did not produce expected output");
    
    trig.trigger();
    context.tick();
    result = stepper.tick(context);
    STAssertEquals(result.value, 1.0f, @"ControlStepper did not produce expected output");
    
    trig.trigger();
    context.tick();
    result = stepper.tick(context);
    STAssertEquals(result.value, 2.0f, @"ControlStepper did not produce expected output");
  }

  
}

- (void)test201ControlStepperSharedTrigger
{
  {
    ControlTrigger trig;
    ControlStepper stepper;
    stepper = ControlStepper().start(1).end(3).step(1).trigger(trig);
    ADSR env = ADSR(1,1,1,1).trigger(trig);
    Tonic_::SynthesisContext_ context;
    
    env.tick(testFrames, context);
    ControlGeneratorOutput result = stepper.tick(context);
    STAssertEquals(result.value, 1.0f, @"ControlStepper did not produce expected output");
    
    trig.trigger();
    context.tick();
    env.tick(testFrames, context);
    result = stepper.tick(context);
    STAssertEquals(result.value, 2.0f, @"ControlStepper did not produce expected output");
    
    
    
    
  }
}

- (void)test202ControlTrigger{
  ControlTrigger trig;
  Tonic_::SynthesisContext_ context;
  STAssertEquals(trig.tick(context).status, ControlGeneratorStatusHasNotChanged, @"ControlGenerator did not produce expected output");
  
  trig.trigger();
  context.tick();
  STAssertEquals(trig.tick(context).status, ControlGeneratorStatusHasChanged, @"ControlGenerator did not produce expected output");
  
  
}


#pragma mark - Buffer filler tests

- (void)test300BufferFillerMonoOutMonoSource
{
  TestBufferFiller testFiller;
  
  // Mono fixed value
  FixedValue fv = FixedValue(0.5);
  testFiller.setOutputGen(fv);
  
  testFiller.fillBufferOfFloats(monoOutBuffer, kTestOutputBlockSize, 1);
  
  [self verifyBufferFillerMonoFixedOutputEquals:0.5];
}

- (void)test301BufferFillerMonoOutStereoSource
{
  TestBufferFiller testFiller;
  
  // Stereo fixed value
  StereoFixedTestGen fv = StereoFixedTestGen(0.5, 1.0);
  testFiller.setOutputGen(fv);
  
  testFiller.fillBufferOfFloats(monoOutBuffer, kTestOutputBlockSize, 1);
  
  // average
  [self verifyBufferFillerMonoFixedOutputEquals:0.75];
}

- (void)test302BufferFillerStereoOutMonoSource
{
  TestBufferFiller testFiller;
  
  // Mono fixed value
  FixedValue fv = FixedValue(0.5);
  testFiller.setOutputGen(fv);
  
  testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
  
  [self verifyBufferFillerStereoFixedOutputEqualsLeft:0.5 right:0.5];
}

- (void)test303BufferFillerStereoOutStereoSource
{
  TestBufferFiller testFiller;
  
  // Stereo fixed value
  StereoFixedTestGen fv = StereoFixedTestGen(0.5, 1.0);
  testFiller.setOutputGen(fv);
  
  testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
  
  [self verifyBufferFillerStereoFixedOutputEqualsLeft:0.5 right:1.0];
  
}

@end
