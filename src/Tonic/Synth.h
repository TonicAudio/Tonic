//
//  Synth.h
//  Tonic
//
//  Created by Morgan Packard on 1/23/13.
//
//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__Synth__
#define __Tonic__Synth__

#include <map>
#include "BufferFiller.h"
#include "ControlParameter.h"
#include "CompressorLimiter.h"
#include "ControlChangeNotifier.h"

namespace Tonic{
  
  class Synth  : public BufferFiller{
    
  public:
    
    Synth();
    
    //! Set the output gen that produces audio for the Synth
    void  setOutputGen(Generator gen);
    
    //! Returns a reference to outputGen
    const Generator getOutputGenerator() { return outputGen; };
    
    //! Set whether synth uses dynamic limiter to prevent clipping/wrapping. Defaults to true.
    void setLimitOutput(bool shouldLimit) { limitOutput_ = shouldLimit; };
    
    //! Add a ControlParameter with name "name"
    ControlParameter & addParameter(string name, TonicFloat initialValue = 0.f);
    
    void                      setParameter(string name, float value=1);
    vector<ControlParameter>  getParameters();
    
    void tick( TonicFrames& frames, const Tonic_::SynthesisContext_ & context );
    
    //! Returns a ControlConditioner which accepts an input and a ControlChangeSubscriber (supplied by the UI).
    //! When the input value changes, ControlChangeSubscriber::messageRecieved is called
    template<class T>
    T exposeToUI(T input, string name){
      ControlChangeNotifier messenger;
      messenger.setName(name);
      messenger.input(input);
      uiMessengers_[name] = messenger;
      addAuxControlGenerator(messenger);
      return input;
    }
    
    /*! In cases where we want ControlGenerator to be ticked, but don't necessarily want to add it to the
      synthesis graph, we can add it here and it will be automatically ticked.
    */
    void addAuxControlGenerator(ControlGenerator);
    void addControlChangeSubscriber(string name, ControlChangeSubscriber* resp);
    void tickUI();
    
  protected:
        
    Generator     outputGen;
    
    Limiter limiter_;
    bool limitOutput_;
    
    std::map<string, ControlParameter> parameters_;
    std::vector<string> orderedParameterNames_;
    std::map<string, ControlChangeNotifier> uiMessengers_; // TODO rename this
    // ControlGenerators that may not be part of the synthesis graph, but should be ticked anyway
    vector<ControlGenerator> auxControlGenerators_;
        
  };
  
  inline void Synth::tick(Tonic::TonicFrames &frames, const Tonic_::SynthesisContext_ &context){
    TONIC_MUTEX_LOCK(&mutex_);
    outputGen.tick(frames, context);
    for (vector<ControlGenerator>::iterator it = auxControlGenerators_.begin(); it != auxControlGenerators_.end(); it++) {
      it->tick(context);
    }
    TONIC_MUTEX_UNLOCK(&mutex_);

    if (limitOutput_){
      limiter_.tickThrough(frames, context);
    }
  }
  
  // ------------------------------
  //
  // Adding factory/registration for easy wrappers and instantiation
  //
  // http://stackoverflow.com/questions/582331/is-there-a-way-to-instantiate-objects-from-a-string-holding-their-class-name
  //
  // -----------------------------
  
  template<typename T> Synth * createSynth() { return new T; }
  
  struct SynthFactory {
    
    typedef std::map<std::string, Synth*(*)()> map_type;
    
    static Synth * createInstance(std::string const& s) {
      map_type::iterator it = getMap()->find(s);
      if(it == getMap()->end()){
        string synthsList = "";
        
        for(it = getMap()->begin(); it != getMap()->end(); it++){
          synthsList = synthsList + it->first + "\n";
        }
        
        error("Error creating synth. Synth named \"" + s + "\" not found. Existing registered synths are: \n" + synthsList);
        
        return 0;
      }
      return it->second();
    }
    
  protected:
    static map_type * getMap() {
      // never delete'ed. (exist until program termination)
      // because we can't guarantee correct destruction order
      if(!map) { map = new map_type; }
      return map;
    }
    
  private:
    static map_type * map;
  };
  
  template<typename T>
  struct SourceRegister : SynthFactory {
    SourceRegister(std::string const& s) {
      getMap()->insert(std::make_pair(s, &createSynth<T>));
    }
  };
  
}

// This macro will auto-register a synth type. Just add it below the Synth class definition.

#define registerSynth(SynthName)              \
                                              \
class SynthName ## _RegistrationPoint {       \
                                              \
protected:                                    \
  static SourceRegister<SynthName> reg;       \
                                              \
};                                            \
                                                                                \
SourceRegister<SynthName> SynthName ## _RegistrationPoint::reg(#SynthName);     \

#endif /* defined(__Tonic__Synth__) */
