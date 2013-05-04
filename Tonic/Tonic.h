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
#include "MonoToStereoPanner.h"
#include "FixedValue.h"
#include "RampedValue.h"
#include "Adder.h"
#include "Subtractor.h"
#include "Multiplier.h"
#include "Divider.h"
#include "ControlAdder.h"
#include "ControlSubtractor.h"
#include "ControlMultiplier.h"
#include "ControlDivider.h"
#include "Synth.h"
#include "MonoToStereoPanner.h"

// Generators
#include "Filters.h"
#include "RectWave.h"
#include "SineWave.h"

// ControlGenerators
#include "ControlPrinter.h"
#include "ControlXYSpeed.h"
#include "StereoDelay.h"
#include "ControlMetro.h"
#include "ADSR.h"
#include "ControlSwitcher.h"
#include "ControlCounter.h"
#include "ControlStepper.h"
#include "ControlRandom.h"



#endif
