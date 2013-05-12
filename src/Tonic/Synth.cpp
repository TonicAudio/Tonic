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
  
  // Synth Factory
  SynthFactory::map_type * SynthFactory::map;
  
  Synth::SynthParameter::SynthParameter() :
    name(""),
    type(SynthParameterTypeContinuous),
    min(-FLT_MIN),
    max(FLT_MAX),
    isLogarithmic(false)
  {}

  Synth::Synth() :
    clampsParameters_(false)
  {}

  ControlValue & Synth::addParameter(string name, float value, float min, float max, bool isLogarithmic){
    return createParameter(SynthParameterTypeContinuous, name, name, value, min, max, isLogarithmic);
  }
  
  ControlValue & Synth::addParameter(string name, string displayName, float value, float min, float max, bool isLogarithmic){
    return createParameter(SynthParameterTypeContinuous, name, displayName, value, min, max, isLogarithmic);
  }
  
  ControlValue  & Synth::addBinaryParameter(string name){
    return addBinaryParameter(name, name);
  }
  
  ControlValue  & Synth::addBinaryParameter(string name, string displayName, bool isMomentary){
    return createParameter(isMomentary ? SynthParameterTypeMomentary : SynthParameterTypeToggle, name, displayName, 0, 0, 1, false);
  }
  
  void Synth::setParameter(string name, float value){
    if (parameters_.find(name)!=parameters_.end()) {
      
      Synth::SynthParameter & param = parameters_[name];
      
      if (clampsParameters_){
        switch (param.type) {
                
          case SynthParameterTypeContinuous:
            param.value.setValue(clamp(value, param.max, param.min));
            break;
            
          case SynthParameterTypeToggle:
          case SynthParameterTypeMomentary:
            param.value.setValue(value <= 0 ? 0 : 1.0f);
            break;
            
          default:
            break;
        }
      }
      else{
        param.value.setValue(value);
      }
      
    }
    else{
      error("message: " + name + " was not registered. You can register a message using Synth::addParameter.");
    }

  }
  
  vector<Synth::SynthParameter> Synth::getParameters(){
    vector<Synth::SynthParameter> returnParams;
    for (std::map<string, SynthParameter>::iterator it = parameters_.begin(); it != parameters_.end(); it++){
      returnParams.push_back(it->second);
    }
    return returnParams;
  }
  
  
  ControlValue & Synth::createParameter(SynthParameterType type, string name, string displayName, float value, float min, float max, bool isLogarithmic){
    if (parameters_.find(name)==parameters_.end()) {
      
      SynthParameter newParam;
      newParam.name = name;
      newParam.displayName = displayName;
      newParam.value.setValue(value);
      newParam.type = type;
      newParam.min = min;
      newParam.max = max;
      newParam.isLogarithmic = isLogarithmic;
      
      parameters_[name] = newParam;
    }
    return parameters_[name].value;
  }
  
}
