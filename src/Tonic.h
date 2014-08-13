//
//  Tonic.h
//  Tonic
//
//  Created by Nick Donaldson on 2/24/13.

//

/***********************************************************
 *
 * Consolidated header file for Tonic core objects
 * These objects are enough to build signal chains
 * using Generators and ControlGenerators with overloaded
 * operators for arithmetic.
 *
 ***********************************************************/

#ifndef TONIC_TONICHEADER_H
#define TONIC_TONICHEADER_H

// ------- Core Objects --------

#include "Tonic/TonicCore.h"
#include "Tonic/TonicFrames.h"
#include "Tonic/SampleTable.h"
#include "Tonic/FixedValue.h"
#include "Tonic/Arithmetic.h"
#include "Tonic/ControlValue.h"
#include "Tonic/ControlTrigger.h"
#include "Tonic/ControlParameter.h"
#include "Tonic/ControlArithmetic.h"
#include "Tonic/ControlComparison.h"
#include "Tonic/MonoToStereoPanner.h"
#include "Tonic/RampedValue.h"
#include "Tonic/Synth.h"
#include "Tonic/Mixer.h"

// -------- Generators ---------

// Oscillators
#include "Tonic/TableLookupOsc.h"
#include "Tonic/SineWave.h"

#include "Tonic/SawtoothWave.h"   // Aliasing
#include "Tonic/TriangleWave.h"   // Aliasing
#include "Tonic/SquareWave.h"     // Aliasing
#include "Tonic/RectWave.h"       // Aliasing

#include "Tonic/Noise.h"

// Effects
#include "Tonic/CombFilter.h"
#include "Tonic/Filters.h"
#include "Tonic/StereoDelay.h"
#include "Tonic/BasicDelay.h"
#include "Tonic/Reverb.h"
#include "Tonic/FilterUtils.h"
#include "Tonic/DelayUtils.h"
#include "Tonic/Reverb.h"
#include "Tonic/BitCrusher.h"

// Utilities
#include "Tonic/ADSR.h"
#include "Tonic/RingBuffer.h"
#include "Tonic/LFNoise.h"

// Non-Oscillator Audio Sources
#include "Tonic/BufferPlayer.h"


// ------- Control Generators --------

#include "Tonic/ControlDelay.h"
#include "Tonic/ControlDbToLinear.h"
#include "Tonic/ControlFloor.h"
#include "Tonic/ControlPulse.h"
#include "Tonic/ControlPrinter.h"
#include "Tonic/ControlXYSpeed.h"
#include "Tonic/ControlMetro.h"
#include "Tonic/ControlMetroDivider.h"
#include "Tonic/ControlSwitcher.h"
#include "Tonic/ControlCounter.h"
#include "Tonic/ControlStepper.h"
#include "Tonic/ControlRandom.h"
#include "Tonic/ControlSnapToScale.h"
#include "Tonic/ControlMidiToFreq.h"
#include "Tonic/ControlTriggerFilter.h"
#include "Tonic/ControlCallback.h" // C++11 only

// -------- Util ---------

#include "Tonic/AudioFileUtils.h"

#endif
