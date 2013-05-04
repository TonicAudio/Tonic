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

#ifndef __Tonic_TonicHeader__
#define __Tonic_TonicHeader__

// basic stuff
#include "Tonic/TonicCore.h"
#include "Tonic/TonicFrames.h"
#include "Tonic/MonoToStereoPanner.h"
#include "Tonic/FixedValue.h"
#include "Tonic/RampedValue.h"
#include "Tonic/Adder.h"
#include "Tonic/Subtractor.h"
#include "Tonic/Multiplier.h"
#include "Tonic/Divider.h"
#include "Tonic/ControlAdder.h"
#include "Tonic/ControlSubtractor.h"
#include "Tonic/ControlMultiplier.h"
#include "Tonic/ControlDivider.h"
#include "Tonic/ADSR.h"
#include "Tonic/MonoToStereoPanner.h"
#include "Tonic/Mixer.h"
#include "Tonic/Synth.h"

// -------  Generators ---------

// Oscillators
#include "Tonic/SineWave.h"
#include "Tonic/TableLookupOsc.h"
#include "Tonic/FlexToothWave.h"
#include "Tonic/SineWave.h"
#include "Tonic/RectWave.h"
#include "Tonic/Noise.h"

// Effects
#include "Tonic/Filters.h"
#include "Tonic/StereoDelay.h"
#include "Tonic/BasicDelay.h"
#include "Tonic/FilterUtils.h"
#include "Tonic/DelayUtils.h"

// Utilities
#include "Tonic/LFNoise.h"

// ------ Control Generators --------

#include "Tonic/ControlTrigger.h"
#include "Tonic/ControlFloor.h"
#include "Tonic/ControlPulse.h"
#include "Tonic/ControlPrinter.h"
#include "Tonic/ControlXYSpeed.h"
#include "Tonic/ControlMetro.h"
#include "Tonic/ControlSwitcher.h"
#include "Tonic/ControlCounter.h"
#include "Tonic/ControlStepper.h"
#include "Tonic/ControlRandom.h"
#include "Tonic/ControlSnapToScale.h"
#include "Tonic/ControlMidiToFreq.h"


#endif
