//
//  Synth.cpp
//  2013_1_23_melody
//
//  Created by Morgan Packard on 1/23/13.
//
//
// See LICENSE.txt for license and usage information.
//


#include "Synth.h"
#include <sstream>

namespace Tonic {
  
  Synth::SynthParameter::SynthParameter() :
    name(""),
    type(SynthParameterTypeContinuous),
    min(-FLT_MIN),
    max(FLT_MAX)
  {};

  Synth::Synth(){
    
  }

  ControlValue & Synth::addParameter(string name, float value, float min, float max){
    return addParameter(name, SynthParameterTypeContinuous, value, min, max);
  }
  
  ControlValue & Synth::addParameter(string name, SynthParameterType type, float value, float min, float max){
    if (parameters.find(name)==parameters.end()) {
      
      SynthParameter newParam;
      newParam.name = name;
      newParam.value.setValue(value);
      newParam.type = type;
      newParam.min = min;
      newParam.max = max;
      
      parameters[name] = newParam;
    }
    return parameters[name].value;
  }
  
  void Synth::setParameter(string name, float value){
    if (parameters.find(name)!=parameters.end()) {
      
      Synth::SynthParameter & param = parameters[name];
      
      switch (param.type) {
              
        case SynthParameterTypeContinuous:
          param.value.setValue(clamp(value, param.min, param.max));
          break;
          
        default:
          break;
      }
      
//MP -- this was giving me compiler errors, and I think it doesn't really need to be in here anyway. Delete?
//      std::stringstream ss;
//      ss << "message: " << name << " value: " << param.value.getValue();
//      
//      debug(ss.str());
      
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
