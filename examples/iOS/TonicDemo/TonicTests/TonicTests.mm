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

  class TestControlChangeSubscriber : public ControlChangeSubscriber{
    public:
    bool valueChangedFlag;
    TestControlChangeSubscriber() : valueChangedFlag(false){}
    void valueChanged(string, TonicFloat){
      valueChangedFlag = true;
    }
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
    XCTAssertEqual(expectedOutput, testFrames[i], @"Did not produce expected output");
    if (testFrames[i] != expectedOutput) break;
  }
}

- (void)verifyStereoFixedOutputEqualsLeft:(float)l right:(float)r
{
  for (unsigned int i=0; i<testFrames.frames(); i++){
    XCTAssertEqual(l, testFrames[2*i], @"Left channel not produce expected output on frame %i", i);
    if (testFrames[2*i] != l) break;
    
    XCTAssertEqual(r, testFrames[2*i+1], @"Right channel did not produce expected output on frame %i", i);
    if (testFrames[2*i+1] != r) break;
  }
}

- (void)verifyBufferFillerMonoFixedOutputEquals:(float)expectedOutput
{
  for (unsigned int i=0; i<kTestOutputBlockSize; i++){
    XCTAssertEqual(expectedOutput, monoOutBuffer[i], @"Did not produce expected output");
    if (monoOutBuffer[i] != expectedOutput) break;
  }
}

- (void)verifyBufferFillerStereoFixedOutputEqualsLeft:(float)l right:(float)r
{
  for (unsigned int i=0; i<kTestOutputBlockSize; i++){
    XCTAssertEqual(l, stereoOutBuffer[2*i], @"Left channel not produce expected output");
    if (stereoOutBuffer[2*i] != l) break;
    
    XCTAssertEqual(r, stereoOutBuffer[2*i+1], @"Right channel did not produce expected output");
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
  
  XCTAssertEqual(testFrames.channels(), (unsigned int)2, @"TestFrames should have 2 channels");
  
  [self verifyFixedOutputEquals:testFloat];
}

- (void)test102TickStereoToMono
{
  // Tick a stereo source to a mono frames object. Ensure mono value is as expected (average of L-R channels)
  
  [self configureStereo:NO];
  
  StereoFixedTestGen stereoVal = StereoFixedTestGen(0.5, 1.0); // 0.5 in left channel, 1.0 in right
  
  XCTAssertTrue(stereoVal.isStereoOutput(), @"StereoFixedTestGen should output stereo");
  
  stereoVal.tick(testFrames, testContext);
  
  // Result should be average, or 0.75
  [self verifyFixedOutputEquals:0.75f];
}

- (void)test103AdderMonoMono
{
  // Add two mono generators, ensure result is mono
  
  [self configureStereo:NO];
  
  Adder monoAdder = FixedValue(0.5) + FixedValue(0.5);
  
  XCTAssertFalse(monoAdder.isStereoOutput(), @"Adder should be mono");
  
  monoAdder.tick(testFrames, testContext);
  
  [self verifyFixedOutputEquals:1.0f];
  
}

- (void)test104AdderMonoStereo
{
  // Add one mono, one stereo, ensure result is stereo
  
  [self configureStereo:YES];
  
  Adder stereoAdder = FixedValue(0.5) + StereoFixedTestGen(0.0, 1.0);
  
  XCTAssertTrue(stereoAdder.isStereoOutput(), @"Adder should be stereo");
  
  stereoAdder.tick(testFrames, testContext);
  
  [self verifyStereoFixedOutputEqualsLeft:0.5f right:1.5f];
  
}

- (void)test105AdderStereoStereo
{
  [self configureStereo:YES];
  
  Adder stereoAdder = StereoFixedTestGen(1.0, 0.0) + StereoFixedTestGen(0.0, 1.0);
  
  XCTAssertTrue(stereoAdder.isStereoOutput(), @"Adder should be stereo");
  
  stereoAdder.tick(testFrames, testContext);
  
  [self verifyFixedOutputEquals:1.0f];
  
}

- (void)test106MultMonoMono
{
  // Multiply two mono generators, ensure result is mono
  
  [self configureStereo:NO];
  
  Multiplier monoMult = FixedValue(0.5) * FixedValue(0.5);
  
  XCTAssertFalse(monoMult.isStereoOutput(), @"Multiplier should be mono");
  
  monoMult.tick(testFrames, testContext);
  
  [self verifyFixedOutputEquals:0.25f];
  
}

- (void)test107MultMonoStereo
{
  // Multiply one mono, one stereo, ensure result is stereo
  
  [self configureStereo:YES];
  
  Multiplier stereoMult = FixedValue(0.5) * StereoFixedTestGen(0.0, 1.0);
  
  XCTAssertTrue(stereoMult.isStereoOutput(), @"Multiplier should be stereo");
  
  stereoMult.tick(testFrames, testContext);
  
  [self verifyStereoFixedOutputEqualsLeft:0.0f right:0.5f];
  
}



- (void)test107_1_MultStereoMono
{
  // Multiply one stereo, one mono, with stereo on the right side, ensure result is stereo
  
  [self configureStereo:YES];
  
  Multiplier stereoMult =  StereoFixedTestGen(0.0, 1.0) * FixedValue(0.5);
  
  XCTAssertTrue(stereoMult.isStereoOutput(), @"Multiplier should be stereo");
  
  stereoMult.tick(testFrames, testContext);
  
  [self verifyStereoFixedOutputEqualsLeft:0.0f right:0.5f];
  
}

- (void)test108MultStereoStereo
{
  [self configureStereo:YES];
  
  Multiplier stereoMult = StereoFixedTestGen(1.0, 0.0) * StereoFixedTestGen(0.0, 1.0);
  
  XCTAssertTrue(stereoMult.isStereoOutput(), @"Multiplier should be stereo");
  
  stereoMult.tick(testFrames, testContext);
  
  // Both channels 0
  [self verifyFixedOutputEquals:0.0f];
  
}


- (void)test1081DivMonoMono
{
  [self configureStereo:NO];
  
  Divider divMono = FixedValue(1) / FixedValue(2);
  
  XCTAssertFalse(divMono.isStereoOutput(), @"Divider should be mono");
  
  divMono.tick(testFrames, testContext);
  
  [self verifyFixedOutputEquals:0.5f];
  
}

- (void)test1082DivGenControlGenCombos
{

  {
    [self configureStereo:NO];
    
    Divider divMono = FixedValue(1) / ControlValue(2);
    
    XCTAssertFalse(divMono.isStereoOutput(), @"Divider should be mono");
    
    divMono.tick(testFrames, testContext);
    
    [self verifyFixedOutputEquals:0.5f];
  }

  {
    [self configureStereo:NO];
    
    Divider divMono = ControlValue(1) / FixedValue(2);
    
    XCTAssertFalse(divMono.isStereoOutput(), @"Divider should be mono");
    
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
  XCTAssertEqual((float)1, *stereoOutBuffer, @"FixedValue(2) - FixedValue(1) failed");
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
 
  XCTAssertTrue(*stereoOutBuffer != 0, @"Limiter shouldn't limit 100 to zero");
  
}

-(void)test120TestBitCrusherStereoPreservation{

  [self configureStereo:YES];
  BitCrusher bitCrusher = BitCrusher().bitDepth(16).input(StereoFixedTestGen(0.0, 1.0));
  XCTAssertTrue(bitCrusher.isStereoOutput(), @"bitcrusher should know that it's strereo output");
  bitCrusher.tick(testFrames, testContext);
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
    XCTAssertEqual(result.value, 1.0f, @"ControlStepper did not produce expected output");
  }
  
    {
    //setup
    ControlTrigger trig;
    ControlStepper stepper;
    stepper = ControlStepper().start(1).end(3).step(1).bidirectional(1).trigger(trig);

    Tonic_::SynthesisContext_ context;
    ControlGeneratorOutput result = stepper.tick(context);
    XCTAssertEqual(result.value, 1.0f, @"ControlStepper did not produce expected output");
    
    trig.trigger();
    context.tick();
    result = stepper.tick(context);
    XCTAssertEqual(result.value, 2.0f, @"ControlStepper did not produce expected output");
    
    trig.trigger();
    context.tick();
    result = stepper.tick(context);
    XCTAssertEqual(result.value, 3.0f, @"ControlStepper did not produce expected output");
    
    trig.trigger();
    context.tick();
    result = stepper.tick(context);
    XCTAssertEqual(result.value, 2.0f, @"ControlStepper did not produce expected output");
    
    trig.trigger();
    context.tick();
    result = stepper.tick(context);
    XCTAssertEqual(result.value, 1.0f, @"ControlStepper did not produce expected output");
    
    trig.trigger();
    context.tick();
    result = stepper.tick(context);
    XCTAssertEqual(result.value, 2.0f, @"ControlStepper did not produce expected output");
    
    
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
    XCTAssertEqual(result.value, 1.0f, @"ControlStepper did not produce expected output");
    
    trig.trigger();
    context.tick();
    env.tick(testFrames, context);
    result = stepper.tick(context);
    XCTAssertEqual(result.value, 2.0f, @"ControlStepper did not produce expected output");
    
    
  }
}

- (void)test202ControlTrigger{
  ControlTrigger trig;
  Tonic_::SynthesisContext_ context;
  XCTAssertFalse(trig.tick(context).triggered, @"ControlGenerator did not produce expected output");
  
  trig.trigger();
  context.tick();
  XCTAssertTrue(trig.tick(context).triggered, @"ControlGenerator did not produce expected output");
  
}

-(void)test203ControlRandom{

  ControlTrigger trig = ControlTrigger();
  ControlRandom random = ControlRandom().min(10).max(20).trigger(trig);
  Tonic_::SynthesisContext_ context;  
  XCTAssertTrue(random.tick(context).value > 0, @"ControlRandom should start out with a value inside its range.");
  
  random.min(1).max(2);
  XCTAssertTrue(random.tick(context).value <= 2, @"ControlRandom should start out with a value inside its range.");
  XCTAssertFalse(random.tick(context).triggered, @"ControlRandom should not produce new trigger unless input is triggered.");
  
  float randVal = random.tick(context).value;
  trig.trigger();
  XCTAssertTrue(random.tick(context).triggered, @"ControlRandom should report change if triggered");
  XCTAssertTrue(random.tick(context).value != randVal, @"ControlRandom should not have the same value after trigger.");
  
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
  XCTAssertFalse( metro.tick(context).triggered, @"Metro shouldn't report trigger.");
  
}

-(void)test205ControlAdder{
  ControlValue left = ControlValue(2);
  ControlValue right = ControlValue(3);
  ControlAdder adder = left + right;
  XCTAssertEqual(adder.tick(testContext).value, 5.0f, @"Adder isn't adding correctly");
  
}


-(void)test206SmoothedChangedStatus{
  ControlValue val(100);
  XCTAssertTrue(val.tick(testContext).triggered, @"Inital status of Control Gen should be 'triggered'");
  
  ControlValue val2(100);
  val2.smoothed();
  XCTAssertTrue(val2.tick(testContext).triggered, @"Inital status of Control Gen should be 'triggered'");
  
  // reset and try again
  testContext.forceNewOutput = true;
  XCTAssertTrue(val2.tick(testContext).triggered, @"Inital status of Control Gen should be 'triggered'");
  
}

-(void)test207ControlAdderAndMultiplier{
  ControlGenerator val1 = ControlValue(2);
  ControlGenerator val2 = ControlValue(2);
  ControlGenerator final = val1 + val1 * val2;
  XCTAssertEqual(final.tick(testContext).value, 6.0f, @"val1 + val1 * val2 doesn't combine correctly with ControlGenerators");
}


-(void)testControlValueTriggering {
  Tonic_::SynthesisContext_ context;
  context.forceNewOutput = false;
  context.tick();
  ControlGenerator val1 = ControlValue(2);
  XCTAssertTrue(val1.tick(context).triggered, @"Fist tick to ControlGen should cause trigger");
  XCTAssertTrue(val1.tick(context).triggered, @"Subsequent ticks (with no context change) should still report triggered.");
  
  context.tick();
  
  XCTAssertFalse(val1.tick(context).triggered, @"Triggered should be false on subsequent ticks after context has advanced.");
 
}

-(void)test208ControlSwitcher{


  Tonic_::SynthesisContext_ localContext;
  
  localContext.forceNewOutput = false;

  ControlTrigger input0Trig;
  ControlTrigger input1Trig;
  ControlValue inputIndex(0);
  ControlSwitcher switcher;
  
  ControlValue index0Val(0);
  ControlValue index1Val(1);
  
  switcher
    .addInput(index0Val)
    .addInput(index1Val)
    .triggerForIndex(input0Trig, 0)
    .triggerForIndex(input1Trig, 1)
    .inputIndex(inputIndex);
  
  ControlGeneratorOutput output;
  
  
  localContext.tick();
  output = switcher.tick(localContext);
  XCTAssertEqual( output.value, index0Val.getValue(), @"InputIndex should be zero");
  
  localContext.tick();
  output = switcher.tick(localContext);
  XCTAssertFalse(output.triggered, @"Second tick with no change should not result in trigger.");
  
  inputIndex.value(1);
  
  localContext.tick();
  output = switcher.tick(localContext);
  XCTAssertEqual( output.value, index1Val.getValue(), @"InputIndex should be one");
  
  index1Val.value(100);
  localContext.tick();
  output = switcher.tick(localContext);
  XCTAssertTrue(output.triggered, @"Change in input value should pass through to output.");
  
  
  localContext.tick();
  output = switcher.tick(localContext);
  XCTAssertFalse(output.triggered, @"Change in input value should pass through to output.");
  
  input0Trig.trigger();
  localContext.tick();
  output = switcher.tick(localContext);
  XCTAssertEqual( output.value, index0Val.getValue(), @"InputIndex should be zero");
  
  input0Trig.trigger();
  localContext.tick();
  output = switcher.tick(localContext);
  XCTAssertFalse( output.triggered, @"Repeat call to same input shouldn't trigger if the input didn't trigger");
  
  input1Trig.trigger();
  localContext.tick();
  output = switcher.tick(localContext);
  XCTAssertTrue( output.triggered, @"Switching inputs should cause a trigger.");
  
  inputIndex.value(0);
  localContext.tick();
  output = switcher.tick(localContext);
  XCTAssertEqual( output.value, index0Val.getValue(), @"Changing the inputIndex should work, even after using an input trigger.");
  
}

-(void)test209ControlSwitcherWrap{

  Tonic_::SynthesisContext_ localContext;
  localContext.forceNewOutput = false;
  
  ControlSwitcher switcher;
  float addAfterWrap = 12;
  float inputZero = 0;
  float inputOne = 2;
  switcher.addInput(inputZero);
  switcher.addInput(inputOne);
  switcher.doesWrap(true);
  switcher.addAfterWrap(addAfterWrap);
  
  
  switcher.inputIndex(0);
  localContext.tick();
  switcher.tick(localContext);
  XCTAssertEqual(switcher.tick(localContext).value, inputZero, @"ControlSwitcher basic test.");
  
  
  switcher.inputIndex(2);
  localContext.tick();
  switcher.tick(localContext);
  XCTAssertEqual(switcher.tick(localContext).value, inputZero + addAfterWrap, @"ControlSwitcher basic test.");
  
  
  switcher.inputIndex(3);
  localContext.tick();
  switcher.tick(localContext);
  XCTAssertEqual(switcher.tick(localContext).value, inputOne + addAfterWrap, @"ControlSwitcher basic test.");
  
  
  switcher.inputIndex(4);
  localContext.tick();
  switcher.tick(localContext);
  XCTAssertEqual(switcher.tick(localContext).value, inputZero + addAfterWrap * 2, @"ControlSwitcher basic test.");
  
  
  switcher.inputIndex(5);
  localContext.tick();
  switcher.tick(localContext);
  XCTAssertEqual(switcher.tick(localContext).value, inputOne + addAfterWrap * 2, @"ControlSwitcher basic test.");
  
  
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

-(void)test304ControlChangeNotifierTest
{

  class TestSynth : public Synth {
    public:
    TestSynth(){
      ControlGenerator random = ControlRandom().trigger(ControlMetro().bpm(1000));
      publishChanges(random, "random");
    }
  };
  
  TestControlChangeSubscriber subscriber;
  TestSynth synth;
  synth.addControlChangeSubscriber("random", &subscriber);
  for(int i = 0; i < 1000; i++){
    synth.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
  }
  
  XCTAssertFalse(subscriber.valueChangedFlag, @"Value changed notification should not have happened");
  subscriber.valueChangedFlag = false;
  synth.sendControlChangesToSubscribers();
  XCTAssertTrue(subscriber.valueChangedFlag, @"Value changed notification should have happened");
  subscriber.valueChangedFlag = false;
  synth.sendControlChangesToSubscribers();
  XCTAssertFalse(subscriber.valueChangedFlag, @"Value changed notification should not have happened");
 
  for(int i = 0; i < 1000; i++){
    synth.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
  }
  
  subscriber.valueChangedFlag = false;
  synth.sendControlChangesToSubscribers();
  XCTAssertTrue(subscriber.valueChangedFlag, @"Value changed notification should have happened");
  
  // now remove the subscriber and make sure it's not notified any more
  
  synth.removeControlChangeSubscriber(&subscriber);
  subscriber.valueChangedFlag = false;
  for(int i = 0; i < 1000; i++){
    synth.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
  }
  synth.sendControlChangesToSubscribers();
  XCTAssertFalse(subscriber.valueChangedFlag, @"Value changed notification should not have happened");
  
  ////////////////////////////
  // Unnamed notifier test
  ////////////////////////////
  
  synth = TestSynth();

  subscriber.valueChangedFlag = false;
  synth.addControlChangeSubscriber(&subscriber);
  for(int i = 0; i < 1000; i++){
    synth.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
  }
  
  XCTAssertFalse(subscriber.valueChangedFlag, @"Value changed notification should not have happened");
  synth.sendControlChangesToSubscribers();
  XCTAssertTrue(subscriber.valueChangedFlag, @"Value changed notification should have happened");
  
}

// publishChanges should return a ControlChangeNotifier which we can subscribe to
// directly, as an alternative to going through Synth::addControlChangeSubscriber
-(void)test304ControlChangeNotifierInlineTest{
  Synth synth;
  const float VALUE = 0.5;
  ControlValue val(VALUE);
  TestControlChangeSubscriber subscriber;
  ControlChangeNotifier notifier = synth.publishChanges(val, "controlValue");
  notifier.addValueChangedSubscriber(&subscriber);
  
  Tonic_::SynthesisContext_ context;
  XCTAssertEqual(notifier.tick(context).value, VALUE, @"A controlChangeNotifier should wrap the ControlGenerator it's observing");
  
  for(int i = 0; i < 1000; i++){
    synth.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
  }
  XCTAssertFalse(subscriber.valueChangedFlag, @"Value changed notification should not have happened");
  synth.sendControlChangesToSubscribers();
  XCTAssertTrue(subscriber.valueChangedFlag, @"Value changed notification should have happened");
  
}

-(void)test304UnNamedControlChangeNotifier{
  Synth synth;
  const float VALUE_1 = 0.5;
  ControlValue val1(VALUE_1);
  
  const float VALUE_2 = 1;
  ControlValue val2(VALUE_2);
  
  TestControlChangeSubscriber subscriber1;
  TestControlChangeSubscriber subscriber2;
  synth.publishChanges(val1).addValueChangedSubscriber(&subscriber1);
  synth.publishChanges(val2).addValueChangedSubscriber(&subscriber2);
  
  for(int i = 0; i < 1000; i++){
    synth.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
  }
  
  synth.sendControlChangesToSubscribers();
  
  XCTAssertTrue(subscriber1.valueChangedFlag, @"Value changed notification should have happened");
  XCTAssertTrue(subscriber2.valueChangedFlag, @"Value changed notification should have happened");

}

#pragma mark operator tests

-(void)test400CombineGeneratorControlGenerator{

  {

  TestBufferFiller testFiller;
  Generator gen =  ControlValue(1) + FixedValue(1);
  testFiller.setOutputGen(gen);
  testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
  XCTAssertEqual(2.f, *stereoOutBuffer, @"ControlValue(1) + FixedValue(1) failed");
  
  }
  
  {

  TestBufferFiller testFiller;
  Generator gen =   FixedValue(1) + ControlValue(1);
  testFiller.setOutputGen(gen);
  testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
  XCTAssertEqual(2.f, *stereoOutBuffer, @"FixedValue(1) + ControlValue(1) failed");
  
  }
  
  {
  
    TestBufferFiller testFiller;
    Generator gen =   FixedValue(3) - ControlValue(2);
    testFiller.setOutputGen(gen);
    testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
    XCTAssertEqual(1.f, *stereoOutBuffer, @"FixedValue(1) + ControlValue(1) failed");

  }
  
  {
  
    TestBufferFiller testFiller;
    Generator gen =   ControlValue(3) - FixedValue(2);
    testFiller.setOutputGen(gen);
    testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
    XCTAssertEqual(1.f, *stereoOutBuffer, @"ControlValue(3) - FixedValue(2) failed");
  }
  
  {
  
    TestBufferFiller testFiller;
    Generator gen =   ControlValue(3) * FixedValue(2);
    testFiller.setOutputGen(gen);
    testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
    XCTAssertEqual(6.f, *stereoOutBuffer, @"ControlValue(3) * FixedValue(2) failed");
  }
  
  {
  
    TestBufferFiller testFiller;
    Generator gen =  FixedValue(3) * ControlValue(2);
    testFiller.setOutputGen(gen);
    testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
    XCTAssertEqual(6.f, *stereoOutBuffer, @" FixedValue(3) * ControlValue(2) failed");
  }
  
}

-(void)test401GeneratorMinusControlGenerator{

  {

  TestBufferFiller testFiller;
  Generator gen =  ControlValue(2) - FixedValue(1);
  testFiller.setOutputGen(gen);
  testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
  XCTAssertEqual(1.f, *stereoOutBuffer, @"ControlValue(2) - FixedValue(1) failed");
  
  }
  
  {

  TestBufferFiller testFiller;
  Generator gen =   FixedValue(2) - ControlValue(1);
  testFiller.setOutputGen(gen);
  testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
  XCTAssertEqual(1.f, *stereoOutBuffer, @"FixedValue(1) - ControlValue(1) failed");
  
  }
  
}

-(void)test402ControlGeneratorDivide{
  {
    ControlGenerator gen = ControlValue(10) / ControlValue(5);
    Tonic_::SynthesisContext_ context;
    XCTAssertEqual(gen.tick(context).value, 2.f, @"ControlValue(10) / ControlValue(5) failed.");
  }

  {
    ControlGenerator gen = 10 / ControlValue(5);
    Tonic_::SynthesisContext_ context;
    XCTAssertEqual(gen.tick(context).value, 2.f, @"10 / ControlValue(5) failed.");
  }

  {
    ControlGenerator gen = ControlValue(10) / 5;
    Tonic_::SynthesisContext_ context;
    XCTAssertEqual(gen.tick(context).value, 2.f, @"ControlValue(10) / 5 failed.");
  }

}

-(void)test403ControlGenDivideByZero{
  ControlValue right = ControlValue(5);
  ControlGenerator gen = ControlValue(10) / right;
  Tonic_::SynthesisContext_ context;
  gen.tick(context);
  right.value(0);
  
  XCTAssertEqual(gen.tick(context).value, 2.f, @"Divide by zero should return the last valid value.");
}


-(void)test404TestCombinationsOfGenAndControlGen{

  TestBufferFiller testFiller;
  ControlGenerator ctrlGen1 = ControlValue(2);
  Generator gen1 = ctrlGen1 + FixedValue(2) * ctrlGen1;
  testFiller.setLimitOutput(false);
  testFiller.setOutputGen(gen1);
  testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
  XCTAssertEqual(*stereoOutBuffer, 6.f, @"Complex combination of control gen and gen failed");
   
  // set the force output flag and try it again to ensure it still works
  testFiller.forceNewOutput();
  testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
  XCTAssertEqual(*stereoOutBuffer, 6.f, @"Complex combination of control gen and gen failed");
  
}

- (void)test405ControlGeneratorComparison {
  
  const int n_iterations = 10;
  Tonic_::SynthesisContext_ context;
  
  // equals
  for (int i=0; i<n_iterations; i++){
    
    float rf = randomFloat(-1000.f, 1000.f);
    
    ControlGenerator g = ControlValue(rf) == ControlValue(rf);
    ControlGenerator g1 = ControlValue(rf) == rf;
    ControlGenerator g2 = ControlValue(rf) == ControlValue(99999.f);
    
    XCTAssertTrue(g.tick(context).triggered, @"Every tick should produce a change");
    
    XCTAssertEqual(g.tick(context).value, 1.0f, @"%.2f should equal itself", rf);
    XCTAssertEqual(g1.tick(context).value, 1.0f, @"%.2f should equal itself", rf);
    XCTAssertEqual(g2.tick(context).value, 0.0f, @"%.2f should not be equal to rhs", rf);

  }
  
  // not equals
  for (int i=0; i<n_iterations; i++){
    
    float rf = randomFloat(-1000.f, 1000.f);
    
    ControlGenerator g = ControlValue(rf) != ControlValue(99999.f);
    ControlGenerator g1 = ControlValue(rf) != 99999.f;
    ControlGenerator g2 = ControlValue(rf) != ControlValue(rf);
    
    XCTAssertTrue(g.tick(context).triggered, @"Every tick should produce a change");
    
    XCTAssertEqual(g.tick(context).value, 1.0f, @"%.2f should not be equal to rhs", rf);
    XCTAssertEqual(g1.tick(context).value, 1.0f, @"%.2f should not be equal to rhs", rf);
    XCTAssertEqual(g2.tick(context).value, 0.0f, @"%.2f should be equal to rhs", rf);
    
  }
  
  // greater than
  for (int i=0; i<n_iterations; i++){
    
    float rf = randomFloat(0.1f, 1000.f);
    
    ControlGenerator g = ControlValue(rf) > ControlValue(0.f);
    ControlGenerator g1 = ControlValue(rf) > 0.f;
    ControlGenerator g2 = ControlValue(rf) > ControlValue(99999.f);
    
    XCTAssertTrue(g.tick(context).triggered, @"Every tick should produce a change");
    
    XCTAssertEqual(g.tick(context).value, 1.0f, @"%.2f should be greater than rhs", rf);
    XCTAssertEqual(g1.tick(context).value, 1.0f, @"%.2f should be greater than rhs", rf);
    XCTAssertEqual(g2.tick(context).value, 0.0f, @"%.2f should not be greater than rhs", rf);
    
  }
  
  // greater than or equal
  for (int i=0; i<n_iterations; i++){
    
    float rf = randomFloat(0.1f, 1000.f);
    
    ControlGenerator g = ControlValue(rf) >= ControlValue(rf);
    ControlGenerator g1 = ControlValue(rf) > 0.f;
    ControlGenerator g2 = ControlValue(rf) >= ControlValue(99999.f);
    
    XCTAssertTrue(g.tick(context).triggered, @"Every tick should produce a change");
    
    XCTAssertEqual(g.tick(context).value, 1.0f, @"%.2f should be greater than or equal to rhs", rf);
    XCTAssertEqual(g1.tick(context).value, 1.0f, @"%.2f should be greater than or equal to rhs", rf);
    XCTAssertEqual(g2.tick(context).value, 0.0f, @"%.2f should not be greater than or equal rhs", rf);
    
  }
  
  // less than
  for (int i=0; i<n_iterations; i++){
    
    float rf = randomFloat(0.1f, 1000.f);
    
    ControlGenerator g = ControlValue(rf) < ControlValue(99999.f);
    ControlGenerator g1 = ControlValue(rf) < 99999.f;
    ControlGenerator g2 = ControlValue(rf) < ControlValue(0.f);
    
    XCTAssertTrue(g.tick(context).triggered, @"Every tick should produce a change");
    
    XCTAssertEqual(g.tick(context).value, 1.0f, @"%.2f should be less than rhs", rf);
    XCTAssertEqual(g1.tick(context).value, 1.0f, @"%.2f should be less than rhs", rf);
    XCTAssertEqual(g2.tick(context).value, 0.0f, @"%.2f should not be less than rhs", rf);
    
  }
  
  // less than or equal
  for (int i=0; i<n_iterations; i++){
    
    float rf = randomFloat(0.1f, 1000.f);
    
    ControlGenerator g = ControlValue(rf) <= ControlValue(rf);
    ControlGenerator g1 = ControlValue(rf) <= 99999.f;
    ControlGenerator g2 = ControlValue(rf) <= ControlValue(0.f);
    
    XCTAssertTrue(g.tick(context).triggered, @"Every tick should produce a change");
    
    XCTAssertEqual(g.tick(context).value, 1.0f, @"%.2f should be less than or equal to rhs", rf);
    XCTAssertEqual(g1.tick(context).value, 1.0f, @"%.2f should be less than or equal to rhs", rf);
    XCTAssertEqual(g2.tick(context).value, 0.0f, @"%.2f should not be less than or equal to rhs", rf);
    
  }
  
}

@end
