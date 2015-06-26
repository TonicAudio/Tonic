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
      limiter_.setIsStereo(true);
    }

    void Synth_::setParameter(string name, float value, bool normalized){
      
      if (parameters_.find(name)!=parameters_.end()) {
        
        ControlParameter & param = parameters_[name];
        
        if (normalized){
          param.setNormalizedValue(value);
        }
        else{
          param.value(value);
        }
        
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
    
    void Synth_::sendControlChangesToSubscribers(){
      vector<ControlChangeNotifier>::iterator it = controlChangeNotifiersList_.begin();
      for (; it != controlChangeNotifiersList_.end(); it++) {
        it->sendControlChangesToSubscribers();
      }
    }
    
    void Synth_::addControlChangeSubscriber(string name, ControlChangeSubscriber* resp){
      controlChangeSubscribers.push_back(pair<ControlChangeSubscriber*, string>(resp, name));
      if (name == "") {
        for(vector<ControlChangeNotifier>::iterator it = controlChangeNotifiersList_.begin(); it != controlChangeNotifiersList_.end(); it++){
          it->addValueChangedSubscriber(resp);
        }
      }else if(controlChangeNotifiers_.find(name) != controlChangeNotifiers_.end()){
        controlChangeNotifiers_[name].addValueChangedSubscriber(resp);
      }
    }
    
    void Synth_::addControlChangeSubscriber(ControlChangeSubscriber* sub){
        addControlChangeSubscriber("", sub);
    }
    
    void Synth_::removeControlChangeSubscriber(ControlChangeSubscriber* sub){
      vector<pair<ControlChangeSubscriber*, string> > newList;
      for (int i = 0; i < controlChangeSubscribers.size(); i++) {
        if (controlChangeSubscribers.at(i).first != sub) {
          newList.push_back(pair<ControlChangeSubscriber*, string>(sub, controlChangeSubscribers.at(i).second));
        }
      }
      controlChangeSubscribers = newList;
    
      vector<ControlChangeNotifier>::iterator it = controlChangeNotifiersList_.begin();
      for (; it != controlChangeNotifiersList_.end(); it++) {
        it->removeValueChangedSubscriber(sub);
      }
    }
    
    ControlChangeNotifier Synth_::publishChanges(ControlGenerator input, string name){
      ControlChangeNotifier messenger;
      messenger.setName(name);
      messenger.input(input);
      controlChangeNotifiersList_.push_back(messenger);
      if(name != ""){
        controlChangeNotifiers_[name] = messenger;
      }
      addAuxControlGenerator(messenger);
      
      // add any previously registered subscribers
      for (int i = 0; i < controlChangeSubscribers.size(); i++) {
      string subsriberName = controlChangeSubscribers.at(i).second;
        if (subsriberName == name || subsriberName == "") {
          messenger.addValueChangedSubscriber(controlChangeSubscribers.at(i).first);
        }
      }
      
      return messenger;
    }
    
  }
  

  
}
