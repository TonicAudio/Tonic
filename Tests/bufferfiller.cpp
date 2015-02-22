//
//  TonicTests.mm
//  TonicTests
//
//  Created by Nick Donaldson on 3/9/13.
//  adapted by Andreas Koerner 2015-02-10
//

#include "tonictests.h"

TESTSUITE(BufferFillerTests, TonicTestcase, "")


  TESTCASE(test300BufferFillerMonoOutMonoSource,"")
    TestBufferFiller testFiller;
    
    // Mono fixed value
    FixedValue fv = FixedValue(0.5);
    testFiller.setOutputGen(fv);
    
    testFiller.fillBufferOfFloats(monoOutBuffer, kTestOutputBlockSize, 1);
    
    TEST(bufferFillerMonoFixedOutputEquals, 0.5);
  END_TESTCASE()

  TESTCASE(test301BufferFillerMonoOutStereoSource,"")
    TestBufferFiller testFiller;
    
    // Stereo fixed value
    StereoFixedTestGen fv = StereoFixedTestGen(0.5, 1.0);
    testFiller.setOutputGen(fv);
    
    testFiller.fillBufferOfFloats(monoOutBuffer, kTestOutputBlockSize, 1);
    
    // average
    TEST(bufferFillerMonoFixedOutputEquals, 0.75);
  END_TESTCASE()

  TESTCASE(test302BufferFillerStereoOutMonoSource,"")
    TestBufferFiller testFiller;
    
    // Mono fixed value
    FixedValue fv = FixedValue(0.5);
    testFiller.setOutputGen(fv);
    
    testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
    
    TEST(bufferFillerStereoFixedOutputEquals, 0.5, 0.5);
  END_TESTCASE()

  TESTCASE(test303BufferFillerStereoOutStereoSource,"")
    TestBufferFiller testFiller;
    
    // Stereo fixed value
    StereoFixedTestGen fv = StereoFixedTestGen(0.5, 1.0);
    testFiller.setOutputGen(fv);
    
    testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
    
    TEST(bufferFillerStereoFixedOutputEquals, 0.5, 1.0);
    
  END_TESTCASE()

  TESTCASE(test304ControlChangeNotifierTest,"")

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
    
    TEST(false, subscriber.valueChangedFlag, "Value changed notification should not have happened");
    subscriber.valueChangedFlag = false;
    synth.sendControlChangesToSubscribers();
    TEST(true, subscriber.valueChangedFlag, "Value changed notification should have happened");
    subscriber.valueChangedFlag = false;
    synth.sendControlChangesToSubscribers();
    TEST(false, subscriber.valueChangedFlag, "Value changed notification should not have happened");
   
    for(int i = 0; i < 1000; i++){
      synth.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
    }
    
    subscriber.valueChangedFlag = false;
    synth.sendControlChangesToSubscribers();
    TEST(true, subscriber.valueChangedFlag, "Value changed notification should have happened");
    
    // now remove the subscriber and make sure it's not notified any more
    
    synth.removeControlChangeSubscriber(&subscriber);
    subscriber.valueChangedFlag = false;
    for(int i = 0; i < 1000; i++){
      synth.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
    }
    synth.sendControlChangesToSubscribers();
    TEST(false, subscriber.valueChangedFlag, "Value changed notification should not have happened");
    
    ////////////////////////////
    // Unnamed notifier test
    ////////////////////////////
    
    synth = TestSynth();

    subscriber.valueChangedFlag = false;
    synth.addControlChangeSubscriber(&subscriber);
    for(int i = 0; i < 1000; i++){
      synth.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
    }
    
    TEST(false, subscriber.valueChangedFlag, "Value changed notification should not have happened");
    synth.sendControlChangesToSubscribers();
    TEST(true, subscriber.valueChangedFlag, "Value changed notification should have happened");
    
  END_TESTCASE()

  // publishChanges should return a ControlChangeNotifier which we can subscribe to
  // directly, as an alternative to going through Synth::addControlChangeSubscriber
  TESTCASE(test304ControlChangeNotifierInlineTest,"")
    Synth synth;
    const float VALUE = 0.5;
    ControlValue val(VALUE);
    TestControlChangeSubscriber subscriber;
    ControlChangeNotifier notifier = synth.publishChanges(val, "controlValue");
    notifier.addValueChangedSubscriber(&subscriber);
    
    Tonic_::SynthesisContext_ context;
    TEST(eq, notifier.tick(context).value, VALUE, "A controlChangeNotifier should wrap the ControlGenerator it's observing");
    
    for(int i = 0; i < 1000; i++){
      synth.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 2);
    }
    TEST(false, subscriber.valueChangedFlag, "Value changed notification should not have happened");
    synth.sendControlChangesToSubscribers();
    TEST(true, subscriber.valueChangedFlag, "Value changed notification should have happened");
    
  END_TESTCASE()

  TESTCASE(test304UnNamedControlChangeNotifier,"")
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
    
    TEST(true, subscriber1.valueChangedFlag, "Value changed notification should have happened");
    TEST(true, subscriber2.valueChangedFlag, "Value changed notification should have happened");

  END_TESTCASE()

END_TESTSUITE()

TONICTEST_MAIN()
