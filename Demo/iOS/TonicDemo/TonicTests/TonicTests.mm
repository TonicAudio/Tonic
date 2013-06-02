//
//  TonicTests.mm
//  TonicTests
//
//  Created by Nick Donaldson on 3/9/13.

//

#import "TonicTests.h"

#include "Tonic.h"
#include "TestBufferFiller.h"
#include "StereoFixedTestGen.h"

#define kTestOutputBlockSize kSynthesisBlockSize*4

using namespace Tonic;

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
    STAssertEquals(expectedOutput, testFrames[i], @"Did not produce expected output");
    if (testFrames[i] != expectedOutput) break;
  }
}

- (void)verifyStereoFixedOutputEqualsLeft:(float)l right:(float)r
{
  for (unsigned int i=0; i<testFrames.frames(); i++){
    STAssertEquals(l, testFrames[2*i], @"Left channel not produce expected output on frame %i", i);
    if (testFrames[2*i] != l) break;
    
    STAssertEquals(r, testFrames[2*i+1], @"Right channel did not produce expected output on frame %i", i);
    if (testFrames[2*i+1] != r) break;
  }
}

- (void)verifyBufferFillerMonoFixedOutputEquals:(float)expectedOutput
{
  for (unsigned int i=0; i<kTestOutputBlockSize; i++){
    STAssertEquals(expectedOutput, monoOutBuffer[i], @"Did not produce expected output");
    if (monoOutBuffer[i] != expectedOutput) break;
  }
}

- (void)verifyBufferFillerStereoFixedOutputEqualsLeft:(float)l right:(float)r
{
  for (unsigned int i=0; i<kTestOutputBlockSize; i++){
    STAssertEquals(l, stereoOutBuffer[2*i], @"Left channel not produce expected output");
    if (stereoOutBuffer[2*i] != l) break;
    
    STAssertEquals(r, stereoOutBuffer[2*i+1], @"Right channel did not produce expected output");
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


- (void)test1081DivMonoMono
{
  [self configureStereo:NO];
  
  Divider divMono = FixedValue(1) / FixedValue(2);
  
  STAssertFalse(divMono.isStereoOutput(), @"Divider should be mono");
  
  divMono.tick(testFrames, testContext);
  
  [self verifyFixedOutputEquals:0.5f];
  
}

- (void)test1082DivGenControlGenCombos
{

  {
    [self configureStereo:NO];
    
    Divider divMono = FixedValue(1) / ControlValue(2);
    
    STAssertFalse(divMono.isStereoOutput(), @"Divider should be mono");
    
    divMono.tick(testFrames, testContext);
    
    [self verifyFixedOutputEquals:0.5f];
  }

  {
    [self configureStereo:NO];
    
    Divider divMono = ControlValue(1) / FixedValue(2);
    
    STAssertFalse(divMono.isStereoOutput(), @"Divider should be mono");
    
    divMono.tick(testFrames, testContext);
    
    [self verifyFixedOutputEquals:0.5f];
  }
    
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

-(void)test110SubtractorTest{
  Generator gen = FixedValue(2) - FixedValue(1);
  TestBufferFiller testFiller;
  testFiller.setOutputGen(gen);
  testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 1);
  STAssertEquals((float)1, *stereoOutBuffer, @"FixedValue(2) - FixedValue(1) failed");
}

// For the limiter output to be observable, limiter must be ticked until the output frame index exceeds the limiter
// lookahead time (default 3ms), otherwise the output will be zero. Lookahead used to reduce distortion.
-(void)testSynthLimiter{
  TestBufferFiller testFiller;
  testFiller.setLimitOutput(true);
  testFiller.setOutputGen(FixedValue(100));
  
  // exceed lookahead of limiter (3 ms lookahead ~ 132 samples
  for (int i=0; i<4; i++)
    testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 1);
 
  STAssertTrue(*stereoOutBuffer != 0, @"Limiter shouldn't limit 100 to zero");
  
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
    stepper = ControlStepper().start(1).end(3).step(1).bidirectional(1).trigger(trig);

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
    
    
    stepper = ControlStepper();
    
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

-(void)test203ControlRandom{

  ControlTrigger trig = ControlTrigger();
  ControlRandom random = ControlRandom().min(10).max(20).trigger(trig);
  Tonic_::SynthesisContext_ context;  
  STAssertTrue(random.tick(context).value > 0, @"ControlRandom should start out with a value inside its range.");
  
  random.min(1).max(2);
  STAssertTrue(random.tick(context).value <= 2, @"ControlRandom should start out with a value inside its range.");
  STAssertTrue(random.tick(context).status == ControlGeneratorStatusHasNotChanged, @"ControlRandom should not change unless triggered.");
  
  float randVal = random.tick(context).value;
  trig.trigger();
  STAssertTrue(random.tick(context).status == ControlGeneratorStatusHasChanged, @"ControlRandom should report change if triggered");
  STAssertTrue(random.tick(context).value != randVal, @"ControlRandom should not have the same value after trigger.");
  
}

-(void)test204ControlMetro{
  ControlMetro metro = ControlMetro().bpm(0.001);
  Tonic_::SynthesisContext_ context;
  context.tick();
  metro.tick(context);
  context.tick();
  metro.tick(context);
  context.tick();
  metro.tick(context);
  context.tick();
  STAssertEquals( metro.tick(context).status, ControlGeneratorStatusHasNotChanged, @"Metro shouldn't report status changed.");
  
}

-(void)test205ControlAdder{
  ControlValue left = ControlValue(2);
  ControlValue right = ControlValue(3);
  ControlAdder adder = left + right;
  STAssertEquals(adder.tick(testContext).value, 5.0f, @"Adder isn't adding correctly");
  
}


-(void)test206SmoothedChangedStatus{
  ControlValue val(100);
  STAssertEquals(val.tick(testContext).status, ControlGeneratorStatusHasChanged, @"Inital status of Control Gen should be 'changed'");
  
  ControlValue val2(100);
  val2.smoothed();
  STAssertEquals(val2.tick(testContext).status, ControlGeneratorStatusHasChanged, @"Inital status of Control Gen should be 'changed'");
  
  // reset and try again
  testContext.forceNewOutput = true;
  STAssertEquals(val2.tick(testContext).status, ControlGeneratorStatusHasChanged, @"Inital status of Control Gen should be 'changed'");
  
}

-(void)test207ControlAdderAndMultiplier{
  ControlGenerator val1 = ControlValue(2);
  ControlGenerator val2 = ControlValue(2);
  ControlGenerator final = val1 + val1 * val2;
  STAssertEquals(final.tick(testContext).value, 6.0f, @"val1 + val1 * val2 doesn't combine correctly with ControlGenerators");
}


-(void)testControlValueHasChangedStatus{
  ControlGenerator val1 = ControlValue(2);
  
  STAssertEquals(val1.tick(testContext).status, ControlGeneratorStatusHasChanged, @"Fist tick to ControlGen should be hasChanged");
  STAssertEquals(val1.tick(testContext).status, ControlGeneratorStatusHasChanged, @"Subsequent ticks (with no context tick) should still report hasChanged.");
 
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

#pragma mark operator tests

-(void)test400CombineGeneratorControlGenerator{

  {

  TestBufferFiller testFiller;
  Generator gen =  ControlValue(1) + FixedValue(1);
  testFiller.setOutputGen(gen);
  testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
  STAssertEquals(2.f, *stereoOutBuffer, @"ControlValue(1) + FixedValue(1) failed");
  
  }
  
  {

  TestBufferFiller testFiller;
  Generator gen =   FixedValue(1) + ControlValue(1);
  testFiller.setOutputGen(gen);
  testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
  STAssertEquals(2.f, *stereoOutBuffer, @"FixedValue(1) + ControlValue(1) failed");
  
  }
  
  {
  
    TestBufferFiller testFiller;
    Generator gen =   FixedValue(3) - ControlValue(2);
    testFiller.setOutputGen(gen);
    testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
    STAssertEquals(1.f, *stereoOutBuffer, @"FixedValue(1) + ControlValue(1) failed");

  }
  
  {
  
    TestBufferFiller testFiller;
    Generator gen =   ControlValue(3) - FixedValue(2);
    testFiller.setOutputGen(gen);
    testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
    STAssertEquals(1.f, *stereoOutBuffer, @"ControlValue(3) - FixedValue(2) failed");
  }
  
  {
  
    TestBufferFiller testFiller;
    Generator gen =   ControlValue(3) * FixedValue(2);
    testFiller.setOutputGen(gen);
    testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
    STAssertEquals(6.f, *stereoOutBuffer, @"ControlValue(3) * FixedValue(2) failed");
  }
  
  {
  
    TestBufferFiller testFiller;
    Generator gen =  FixedValue(3) * ControlValue(2);
    testFiller.setOutputGen(gen);
    testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
    STAssertEquals(6.f, *stereoOutBuffer, @" FixedValue(3) * ControlValue(2) failed");
  }
  
}

-(void)test401GeneratorMinusControlGenerator{

  {

  TestBufferFiller testFiller;
  Generator gen =  ControlValue(2) - FixedValue(1);
  testFiller.setOutputGen(gen);
  testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
  STAssertEquals(1.f, *stereoOutBuffer, @"ControlValue(2) - FixedValue(1) failed");
  
  }
  
  {

  TestBufferFiller testFiller;
  Generator gen =   FixedValue(2) - ControlValue(1);
  testFiller.setOutputGen(gen);
  testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
  STAssertEquals(1.f, *stereoOutBuffer, @"FixedValue(1) - ControlValue(1) failed");
  
  }
  
}

-(void)test402ControlGeneratorDivide{
  {
    ControlGenerator gen = ControlValue(10) / ControlValue(5);
    Tonic_::SynthesisContext_ context;
    STAssertEquals(gen.tick(context).value, 2.f, @"ControlValue(10) / ControlValue(5) failed.");
  }

  {
    ControlGenerator gen = 10 / ControlValue(5);
    Tonic_::SynthesisContext_ context;
    STAssertEquals(gen.tick(context).value, 2.f, @"10 / ControlValue(5) failed.");
  }

  {
    ControlGenerator gen = ControlValue(10) / 5;
    Tonic_::SynthesisContext_ context;
    STAssertEquals(gen.tick(context).value, 2.f, @"ControlValue(10) / 5 failed.");
  }

}

-(void)test403ControlGenDivideByZero{
  ControlValue right = ControlValue(5);
  ControlGenerator gen = ControlValue(10) / right;
  Tonic_::SynthesisContext_ context;
  gen.tick(context);
  right.value(0);
  
  STAssertEquals(gen.tick(context).value, 2.f, @"Divide by zero should return the last valid value.");
}


-(void)test404TestCombinationsOfGenAndControlGen{

  TestBufferFiller testFiller;
  ControlGenerator ctrlGen1 = ControlValue(2);
  Generator gen1 = ctrlGen1 + FixedValue(2) * ctrlGen1;
  testFiller.setLimitOutput(false);
  testFiller.setOutputGen(gen1);
  testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
  STAssertEquals(*stereoOutBuffer, 6.f, @"Complex combination of control gen and gen failed");
   
  // set the force output flag and try it again to ensure it still works
  testFiller.forceNewOutput();
  testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
  STAssertEquals(*stereoOutBuffer, 6.f, @"Complex combination of control gen and gen failed");
  
}

- (void)test405ControlGeneratorBooleanLogic{
  
  Tonic_::SynthesisContext_ context;
  
  // equals
  const int n_iterations = 10;
  for (int i=0; i<n_iterations; i++){
    
    float rf = randomFloat(-1000.f, 1000.f);
    
    ControlGenerator g = ControlValue(rf) == ControlValue(rf);
    ControlGenerator g1 = ControlValue(rf) == rf;
    ControlGenerator g2 = ControlValue(rf) == ControlValue(99999.f);
    
    STAssertTrue(g.tick(context).status == ControlGeneratorStatusHasChanged, @"Every tick should produce a change");
    
    STAssertEquals(g.tick(context).value, 1.0f, @"%.2f should equal itself", rf);
    STAssertEquals(g1.tick(context).value, 1.0f, @"%.2f should equal itself", rf);
    STAssertEquals(g2.tick(context).value, 0.0f, @"%.2f should not be equal to rhs", rf);

  }
  
  // not equals
  for (int i=0; i<n_iterations; i++){
    
    float rf = randomFloat(-1000.f, 1000.f);
    
    ControlGenerator g = ControlValue(rf) != ControlValue(99999.f);
    ControlGenerator g1 = ControlValue(rf) != 99999.f;
    ControlGenerator g2 = ControlValue(rf) != ControlValue(rf);
    
    STAssertTrue(g.tick(context).status == ControlGeneratorStatusHasChanged, @"Every tick should produce a change");
    
    STAssertEquals(g.tick(context).value, 1.0f, @"%.2f should not be equal to rhs", rf);
    STAssertEquals(g1.tick(context).value, 1.0f, @"%.2f should not be equal to rhs", rf);
    STAssertEquals(g2.tick(context).value, 0.0f, @"%.2f should be equal to rhs", rf);
    
  }
}

@end
