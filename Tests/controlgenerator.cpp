//
//  TonicTests.mm
//  TonicTests
//
//  Created by Nick Donaldson on 3/9/13.
//  adapted by Andreas Koerner 2015-02-10
//

#include "tonictests.h"

TESTSUITE(ControlGeneratorTests, TonicTestcase, "")

  TESTCASE(test200ControlStepper, "")
    // there's a bug involving ControlStepper where it appears to output zeros when it shares an metro with another controlgenerator
    
    {
      ControlTrigger trig;
      ControlStepper stepper;
      stepper = ControlStepper().start(1).end(2).step(1).trigger(trig);
      Tonic_::SynthesisContext_ context;
      ControlGeneratorOutput result = stepper.tick(context);
      TEST(eq, result.value, 1.0f, "ControlStepper did not produce expected output");
    }
    
    {
      //setup
      ControlTrigger trig;
      ControlStepper stepper;
      stepper = ControlStepper().start(1).end(3).step(1).bidirectional(1).trigger(trig);

      Tonic_::SynthesisContext_ context;
      ControlGeneratorOutput result = stepper.tick(context);
      TEST(eq, result.value, 1.0f, "ControlStepper did not produce expected output");
      
      trig.trigger();
      context.tick();
      result = stepper.tick(context);
      TEST(eq, result.value, 2.0f, "ControlStepper did not produce expected output");
      
      trig.trigger();
      context.tick();
      result = stepper.tick(context);
      TEST(eq, result.value, 3.0f, "ControlStepper did not produce expected output");
      
      trig.trigger();
      context.tick();
      result = stepper.tick(context);
      TEST(eq, result.value, 2.0f, "ControlStepper did not produce expected output");
      
      trig.trigger();
      context.tick();
      result = stepper.tick(context);
      TEST(eq, result.value, 1.0f, "ControlStepper did not produce expected output");
      
      trig.trigger();
      context.tick();
      result = stepper.tick(context);
      TEST(eq, result.value, 2.0f, "ControlStepper did not produce expected output");
      
      
      stepper = ControlStepper();
      
    }

  END_TESTCASE()

  TESTCASE(test201ControlStepperSharedTrigger, "")
    {
      ControlTrigger trig;
      ControlStepper stepper;
      stepper = ControlStepper().start(1).end(3).step(1).trigger(trig);
      ADSR env = ADSR(1,1,1,1).trigger(trig);
      Tonic_::SynthesisContext_ context;
      
      env.tick(testFrames, context);
      ControlGeneratorOutput result = stepper.tick(context);
      TEST(eq, result.value, 1.0f, "ControlStepper did not produce expected output");
      
      trig.trigger();
      context.tick();
      env.tick(testFrames, context);
      result = stepper.tick(context);
      TEST(eq, result.value, 2.0f, "ControlStepper did not produce expected output");
      
      
    }
  END_TESTCASE()

  TESTCASE(test202ControlTrigger, "")
    ControlTrigger trig;
    Tonic_::SynthesisContext_ context;
    TEST(false, trig.tick(context).triggered, "ControlGenerator did not produce expected output");
    
    trig.trigger();
    context.tick();
    TEST(true, trig.tick(context).triggered, "ControlGenerator did not produce expected output");
  END_TESTCASE()

  TESTCASE(test203ControlRandom, "")

    ControlTrigger trig = ControlTrigger();
    ControlRandom random = ControlRandom().min(10).max(20).trigger(trig);
    Tonic_::SynthesisContext_ context;  
    TEST(true, random.tick(context).value > 0, "ControlRandom should start out with a value inside its range.");
    
    random.min(1).max(2);
    TEST(true, random.tick(context).value <= 2, "ControlRandom should start out with a value inside its range.");
    TEST(false, random.tick(context).triggered, "ControlRandom should not produce new trigger unless input is triggered.");
    
    float randVal = random.tick(context).value;
    trig.trigger();
    TEST(true, random.tick(context).triggered, "ControlRandom should report change if triggered");
    TEST(true, random.tick(context).value != randVal, "ControlRandom should not have the same value after trigger.");
    
  END_TESTCASE()

  TESTCASE(test204ControlMetro, "")
    ControlMetro metro = ControlMetro().bpm(0.001);
    Tonic_::SynthesisContext_ context;
    context.tick();
    metro.tick(context);
    context.tick();
    metro.tick(context);
    context.tick();
    metro.tick(context);
    context.tick();
    TEST(false,  metro.tick(context).triggered, "Metro shouldn't report trigger.");
    
  
  END_TESTCASE()

  TESTCASE(test205ControlAdder, "")
    ControlValue left = ControlValue(2);
    ControlValue right = ControlValue(3);
    ControlAdder adder = left + right;
    TEST(eq, adder.tick(testContext).value, 5.0f, "Adder isn't adding correctly");
    
  
  END_TESTCASE()


  TESTCASE(test206SmoothedChangedStatus, "")
    ControlValue val(100);
    TEST(true, val.tick(testContext).triggered, "Inital status of Control Gen should be 'triggered'");
    
    ControlValue val2(100);
    val2.smoothed();
    TEST(true, val2.tick(testContext).triggered, "Inital status of Control Gen should be 'triggered'");
    
    // reset and try again
    testContext.forceNewOutput = true;
    TEST(true, val2.tick(testContext).triggered, "Inital status of Control Gen should be 'triggered'");
    
  
  END_TESTCASE()

  TESTCASE(test207ControlAdderAndMultiplier, "")
    ControlGenerator val1 = ControlValue(2);
    ControlGenerator val2 = ControlValue(2);
    ControlGenerator final = val1 + val1 * val2;
    TEST(eq, final.tick(testContext).value, 6.0f, "val1 + val1 * val2 doesn't combine correctly with ControlGenerators");
  
  END_TESTCASE()


  TESTCASE(testControlValueTriggering, "")
    Tonic_::SynthesisContext_ context;
    context.forceNewOutput = false;
    context.tick();
    ControlGenerator val1 = ControlValue(2);
    TEST(true, val1.tick(context).triggered, "Fist tick to ControlGen should cause trigger");
    TEST(true, val1.tick(context).triggered, "Subsequent ticks (with no context change) should still report triggered.");
    
    context.tick();
    
    TEST(false, val1.tick(context).triggered, "Triggered should be false on subsequent ticks after context has advanced.");
   
  
  END_TESTCASE()

  TESTCASE(test208ControlSwitcher, "")

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
    TEST(eq,  output.value, index0Val.getValue(), "InputIndex should be zero");
    
    localContext.tick();
    output = switcher.tick(localContext);
    TEST(false, output.triggered, "Second tick with no change should not result in trigger.");
    
    inputIndex.value(1);
    
    localContext.tick();
    output = switcher.tick(localContext);
    TEST(eq,  output.value, index1Val.getValue(), "InputIndex should be one");
    
    index1Val.value(100);
    localContext.tick();
    output = switcher.tick(localContext);
    TEST(true, output.triggered, "Change in input value should pass through to output.");
    
    
    localContext.tick();
    output = switcher.tick(localContext);
    TEST(false, output.triggered, "Change in input value should pass through to output.");
    
    input0Trig.trigger();
    localContext.tick();
    output = switcher.tick(localContext);
    TEST(eq,  output.value, index0Val.getValue(), "InputIndex should be zero");
    
    input0Trig.trigger();
    localContext.tick();
    output = switcher.tick(localContext);
    TEST(false,  output.triggered, "Repeat call to same input shouldn't trigger if the input didn't trigger");
    
    input1Trig.trigger();
    localContext.tick();
    output = switcher.tick(localContext);
    TEST(true,  output.triggered, "Switching inputs should cause a trigger.");
    
    inputIndex.value(0);
    localContext.tick();
    output = switcher.tick(localContext);
    TEST(eq,  output.value, index0Val.getValue(), "Changing the inputIndex should work, even after using an input trigger.");
    
  
  END_TESTCASE()

  TESTCASE(test209ControlSwitcherWrap, "")

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
    TEST(eq, switcher.tick(localContext).value, inputZero, "ControlSwitcher basic test.");
    
    
    switcher.inputIndex(2);
    localContext.tick();
    switcher.tick(localContext);
    TEST(eq, switcher.tick(localContext).value, inputZero + addAfterWrap, "ControlSwitcher basic test.");
    
    
    switcher.inputIndex(3);
    localContext.tick();
    switcher.tick(localContext);
    TEST(eq, switcher.tick(localContext).value, inputOne + addAfterWrap, "ControlSwitcher basic test.");
    
    
    switcher.inputIndex(4);
    localContext.tick();
    switcher.tick(localContext);
    TEST(eq, switcher.tick(localContext).value, inputZero + addAfterWrap * 2, "ControlSwitcher basic test.");
    
    
    switcher.inputIndex(5);
    localContext.tick();
    switcher.tick(localContext);
    TEST(eq, switcher.tick(localContext).value, inputOne + addAfterWrap * 2, "ControlSwitcher basic test.");

  END_TESTCASE()

END_TESTSUITE()

TONICTEST_MAIN()

