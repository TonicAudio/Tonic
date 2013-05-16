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

  Synth::Synth() {
    pthread_mutex_init(&outputGenMutex_, NULL);
    outputGen = PassThroughGenerator();
  }
  
  Synth::~Synth() {
    pthread_mutex_destroy(&outputGenMutex_);
  }
  
  void Synth::setOutputGen(Tonic::Generator gen){
    pthread_mutex_lock(&outputGenMutex_);
    outputGen = gen;
    pthread_mutex_unlock(&outputGenMutex_);
  }

  void Synth::setParameter(string name, float value){
    
    if (parameters_.find(name)!=parameters_.end()) {
      
      ControlParameter & param = parameters_[name];
      param.value(value);
      
    }
    else{
      error("message: " + name + " was not registered. You can register a message using Synth::addParameter.");
    }

  }
  
  ControlParameter & Synth::addParameter(string name, TonicFloat initialValue)
  {
    if (parameters_.find(name) == parameters_.end())
    {
      ControlParameter param = ControlParameter(name).value(initialValue).displayName(name);
      parameters_[name] = param;
      orderedParameterNames_.push_back(name);
    }
    return parameters_[name];
  }
  
  vector<ControlParameter> Synth::getParameters(){
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
