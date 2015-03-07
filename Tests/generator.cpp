//
//  TonicTests.mm
//  TonicTests
//
//  Created by Nick Donaldson on 3/9/13.
//  adapted by Andreas Koerner 2015-02-10
//

#include "tonictests.h"

TESTSUITE(GeneratorTests, TonicTestcase, "")

  TESTCASE(test100FixedValue, "")

    // See if FixedValue is correctly filled with a float
    
    configureStereo(false);

    float testFloat = 0.533;
    FixedValue testVal = FixedValue(testFloat);
    
    testVal.tick(testFrames, testContext);

    TEST(fixedOutputEquals, testFrames, testFloat);
  
  END_TESTCASE()

  TESTCASE(test101TickMonoToStereo, "")
    // Tick a mono source to a stereo frames object. Ensure both channels equal.
    
    configureStereo(true);
    
    float testFloat = 0.533;
    FixedValue testVal = FixedValue(testFloat);
    
    testVal.tick(testFrames, testContext);
    
    TEST(eq, testFrames.channels(), (unsigned int)2, "TestFrames should have 2 channels");
    
    TEST(stereoFixedOutputEquals, testFrames, testFloat, testFloat);
  END_TESTCASE()

  TESTCASE(test102TickStereoToMono, "")
    // Tick a stereo soqrce to a mono frames object. Ensure mono value is as expected (average of L-R channels)
    
    configureStereo(false);
    
    StereoFixedTestGen stereoVal = StereoFixedTestGen(0.5, 1.0); // 0.5 in left channel, 1.0 in right
    
    TEST(true, stereoVal.isStereoOutput(), "StereoFixedTestGen should output stereo");
    
    stereoVal.tick(testFrames, testContext);
    
    // Result should be average, or 0.75
    TEST(fixedOutputEquals, testFrames, 0.75f);
  END_TESTCASE()

  TESTCASE(test103AdderMonoMono, "")
    // Add two mono generators, ensure result is mono
    
    configureStereo(false);
    
    Adder monoAdder = FixedValue(0.5) + FixedValue(0.5);
    
    TEST(false, monoAdder.isStereoOutput(), "Adder should be mono");
    
    monoAdder.tick(testFrames, testContext);
    
    TEST(fixedOutputEquals, testFrames, 1.0f);
    
  END_TESTCASE()

  TESTCASE(test104AdderMonoStereo, "")
    // Add one mono, one stereo, ensure result is stereo
    
    configureStereo(true);
    
    Adder stereoAdder = FixedValue(0.5) + StereoFixedTestGen(0.0, 1.0);
    
    TEST(true, stereoAdder.isStereoOutput(), "Adder should be stereo");
    
    stereoAdder.tick(testFrames, testContext);
    
    TEST(stereoFixedOutputEquals, testFrames, 0.5f, 1.5f);
    
  END_TESTCASE()

  TESTCASE(test105AdderStereoStereo, "")
    configureStereo(true);
    
    Adder stereoAdder = StereoFixedTestGen(1.0, 0.0) + StereoFixedTestGen(0.0, 1.0);
    
    TEST(true, stereoAdder.isStereoOutput(), "Adder should be stereo");
    
    stereoAdder.tick(testFrames, testContext);
    
    TEST(stereoFixedOutputEquals, testFrames, 1.0f, 1.0f);
    
  END_TESTCASE()

  TESTCASE(test106MultMonoMono, "")
    // Multiply two mono generators, ensure result is mono
    
    configureStereo(false);
    
    Multiplier monoMult = FixedValue(0.5) * FixedValue(0.5);
    
    TEST(false, monoMult.isStereoOutput(), "Multiplier should be mono");
    
    monoMult.tick(testFrames, testContext);
    
    TEST(fixedOutputEquals, testFrames, 0.25f);
    
  END_TESTCASE()

  TESTCASE(test107MultMonoStereo, "")
    // Multiply one mono, one stereo, ensure result is stereo
    
    configureStereo(true);
    
    Multiplier stereoMult = FixedValue(0.5) * StereoFixedTestGen(0.0, 1.0);
    
    TEST(true, stereoMult.isStereoOutput(), "Multiplier should be stereo");
    
    stereoMult.tick(testFrames, testContext);
    
    TEST(stereoFixedOutputEquals, testFrames, 0.0f, 0.5f);
    
  END_TESTCASE()



  TESTCASE(test107_1_MultStereoMono, "")
    // Multiply one stereo, one mono, with stereo on the right side, ensure result is stereo
    
    configureStereo(true);
    
    Multiplier stereoMult =  StereoFixedTestGen(0.0, 1.0) * FixedValue(0.5);
    
    TEST(true, stereoMult.isStereoOutput(), "Multiplier should be stereo");
    
    stereoMult.tick(testFrames, testContext);
    
    TEST(stereoFixedOutputEquals, testFrames, 0.0f, 0.5f);
    
  END_TESTCASE()

  TESTCASE(test108MultStereoStereo, "")
    configureStereo(true);
    
    Multiplier stereoMult = StereoFixedTestGen(1.0, 0.0) * StereoFixedTestGen(0.0, 1.0);
    
    TEST(true, stereoMult.isStereoOutput(), "Multiplier should be stereo");
    
    stereoMult.tick(testFrames, testContext);
    
    // Both channels 0
    TEST(stereoFixedOutputEquals, testFrames, 0.0f, 0.0f);
    
  END_TESTCASE()


  TESTCASE(test1081DivMonoMono, "")
    configureStereo(false);
    
    Divider divMono = FixedValue(1) / FixedValue(2);
    
    TEST(false, divMono.isStereoOutput(), "Divider should be mono");
    
    divMono.tick(testFrames, testContext);
    
    TEST(fixedOutputEquals, testFrames, 0.5f);
    
  END_TESTCASE()

  TESTCASE(test1082DivGenControlGenCombos, "")

    {
      configureStereo(false);
      
      Divider divMono = FixedValue(1) / ControlValue(2);
      
      TEST(false, divMono.isStereoOutput(), "Divider should be mono");
      
      divMono.tick(testFrames, testContext);
      
      TEST(fixedOutputEquals, testFrames, 0.5f);
    }

    {
      configureStereo(false);
      
      Divider divMono = ControlValue(1) / FixedValue(2);
      
      TEST(false, divMono.isStereoOutput(), "Divider should be mono");
      
      divMono.tick(testFrames, testContext);
      
      TEST(fixedOutputEquals, testFrames, 0.5f);
    }
      
  END_TESTCASE()

  TESTCASE(test109PannerMonoToStereo, "")
    
    configureStereo(true);
    
    MonoToStereoPanner panner = MonoToStereoPanner().pan(-1).input(FixedValue(1));
    panner.tick(testFrames, testContext);
    TEST(stereoFixedOutputEquals, testFrames, 1, 0);
    
    panner.pan(0);
    panner.tick(testFrames, testContext);
    TEST(stereoFixedOutputEquals, testFrames, 1, 1);
    
    panner.pan(1);
    panner.tick(testFrames, testContext);
    TEST(stereoFixedOutputEquals, testFrames, 0, 1);
    
  END_TESTCASE()

  TESTCASE(test110SubtractorTest, "")
    Generator gen = FixedValue(2) - FixedValue(1);
    TestBufferFiller testFiller;
    testFiller.setOutputGen(gen);
    testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 1);
    TEST(eq, (float)1, *stereoOutBuffer, "FixedValue(2) - FixedValue(1) failed");
  END_TESTCASE()

  // For the limiter output to be observable, limiter must be ticked until the output frame index exceeds the limiter
  // lookahead time (default 3ms), otherwise the output will be zero. Lookahead used to reduce distortion.
  TESTCASE(testSynthLimiter, "")
    TestBufferFiller testFiller;
    testFiller.setLimitOutput(true);
    testFiller.setOutputGen(FixedValue(100));
    
    // exceed lookahead of limiter (3 ms lookahead ~ 132 samples
    for (int i=0; i<4; i++)
      testFiller.fillBufferOfFloats(stereoOutBuffer, kTestOutputBlockSize, 1);
   
    TEST(true, *stereoOutBuffer != 0, "Limiter shouldn't limit 100 to zero");
    
  END_TESTCASE()

  TESTCASE(test120TestBitCrusherStereoPreservation, "")

    configureStereo(true);
    BitCrusher bitCrusher = BitCrusher().bitDepth(16).input(StereoFixedTestGen(0.0, 1.0));
    TEST(true, bitCrusher.isStereoOutput(), "bitcrusher should know that it's strereo output");
    bitCrusher.tick(testFrames, testContext);
    TEST(stereoFixedOutputEquals, testFrames, 0, 1);
  END_TESTCASE()


  TESTCASE(test200TestMixerPreserversStereo, ""){

    MonoToStereoPanner stereoSignal = MonoToStereoPanner();
    Synth synth;
    synth.setOutputGen(stereoSignal);
    
    TEST(true, synth.isStereoOutput(), "Synth should know that it's stereo.");
    
    Mixer mixer;
    mixer.addInput(synth);

    TEST(true, mixer.isStereoOutput(), "Mixer should know that it's stereo.");

  }END_TESTCASE()

END_TESTSUITE()

TONICTEST_MAIN()