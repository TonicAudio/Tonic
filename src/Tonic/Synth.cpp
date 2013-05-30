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

  namespace Tonic_ {
    Synth_::Synth_() : limitOutput_(true) {
      outputGen_ = PassThroughGenerator();
     // limiter_.setIsStereo(true);
    }

    void Synth_::setParameter(string name, float value){
      
      if (parameters_.find(name)!=parameters_.end()) {
        
        ControlParameter & param = parameters_[name];
        param.value(value);
        
      }
      else{
        error("message: " + name + " was not registered. You can register a message using Synth::addParameter.");
      }

    }
    
    ControlParameter Synth_::addParameter(string name, TonicFloat initialValue)
    {
      if (parameters_.find(name) == parameters_.end())
      {
        ControlParameter param = ControlParameter().name(name).value(initialValue).displayName(name);
        parameters_[name] = param;
        orderedParameterNames_.push_back(name);
      }
      return parameters_[name];
    }
    
    void Synth_::addParameter(ControlParameter parameter){
      string name = parameter.getName();
      parameters_[name] = parameter;
      orderedParameterNames_.push_back(name);
    }
    
    void Synth_::addParametersFromSynth(Synth synth){
      vector<ControlParameter> params = synth.getParameters();
      for (unsigned int i=0; i<params.size(); i++)
        addParameter(params[i]);
    }
    
    vector<ControlParameter> Synth_::getParameters(){
      vector<ControlParameter> returnParams;
      for (std::vector<string>::iterator it = orderedParameterNames_.begin(); it != orderedParameterNames_.end(); it++){
        std::map<string, ControlParameter>::iterator paramIt = parameters_.find(*it);
        if (paramIt != parameters_.end()){
          returnParams.push_back(paramIt->second);
        }
      }
      return returnParams;
    }
  }
  
  // TODO I think this should use a mutex
  void Synth::addAuxControlGenerator(ControlGenerator gen){
    TONIC_MUTEX_LOCK(&mutex_);
    auxControlGenerators_.push_back(gen);
    TONIC_MUTEX_UNLOCK(&mutex_);
  }
  
  void Synth::tickUI(){
    std::map<string, ControlChangeNotifier>::iterator it = uiMessengers_.begin();
    for (; it != uiMessengers_.end(); it++) {
      it->second.tickUI();
    }
  }
  
  void Synth::addControlChangeSubscriber(string name, ControlChangeSubscriber* resp){
    if(uiMessengers_.find(name) != uiMessengers_.end()){
      uiMessengers_[name].setValueChangedCallback(resp);
    }else{
      error("No value called " + name + " was exposed to the UI.");
    }
  }
  
  
}
