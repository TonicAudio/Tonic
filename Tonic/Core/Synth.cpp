//
//  Synth.cpp
//  2013_1_23_melody
//
//  Created by Morgan Packard on 1/23/13.
//
//

/*+++++++++++++++++++++ License ++++++++++++++++++++

Use this code for whatever you want. There are NO 
RESTRICTIONS WHATSOVER. Modify it, repackage it, 
sell it, get rich from it, whatever. Go crazy. If 
you want to make mehappy, contribute to this 
project, but feel free to just use the code as a 
starting point for whatever you like.

Note that Tonic is heavily indebted to STK
https://ccrma.stanford.edu/software/stk/

++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "Synth.h"
#include "SineWave.h"

namespace Tonic {
  
  Synth::SynthParameter::SynthParameter() :
    name(""),
    type(SynthParameterTypeContinuous),
    min(-FLT_MIN),
    max(FLT_MAX),
    increment(0)
  {};

  Synth::Synth(){
    
  }

  Synth::SynthParameter & Synth::addParameter(string name, float value, float min, float max){
    return addParameter(name, SynthParameterTypeContinuous, value, min, max);
  }
  
  Synth::SynthParameter & Synth::addParameter(string name, SynthParameterType type, float value, float min, float max, float inc){
    if (parameters.find(name)==parameters.end()) {
      
      SynthParameter newParam;
      newParam.value.setValue(value);
      newParam.type = type;
      newParam.min = min;
      newParam.max = max;
      newParam.increment = inc;
      
      parameters[name] = newParam;
    }
    return parameters[name];
  }
  
  void Synth::setParameter(string name, float value){
    if (parameters.find(name)!=parameters.end()) {
      
      Synth::SynthParameter & param = parameters[name];
      
      switch (param.type) {
        case SynthParameterTypeContinuous:
          param.value.setValue(clamp(value, param.min, param.max));
          break;
          
        case SynthParameterTypeIncremental:
        {
          if (param.increment != 0){
            float r = floorf((value - param.min)/param.increment);
            r = param.min + r * param.increment;
            param.value.setValue(clamp(r, param.min, param.max));
          }
          else{
            param.value.setValue(clamp(value, param.min, param.max));
          }
        }
          break;
          
        case SynthParameterTypeToggle:
          param.value.setValue(param.value.getValue() == 0 ? 1 : 0);
          break;
          
        default:
          break;
      }
      
      
      stringstream ss;
      ss << "message: " << name << " value: " << param.value.getValue();
      
      debug(ss.str());
      
    }
    else{
      error("message: " + name + " was not registered. You can register a message using Synth::addParameter.");
    }

  }
  
  vector<Synth::SynthParameter> Synth::getParameters(){
    vector<Synth::SynthParameter> returnParams;
    for (std::map<string, SynthParameter>::iterator it = parameters.begin(); it != parameters.end(); it++){
      returnParams.push_back(it->second);
    }
    return returnParams;
  }
  
  // Synth Factory
  SynthFactory::map_type * SynthFactory::map;
  
}
