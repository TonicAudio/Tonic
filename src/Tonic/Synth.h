//
//  Synth.h
//  Tonic
//
//  Created by Morgan Packard on 1/23/13.
//
//
// See LICENSE.txt for license and usage information.
//


#ifndef TONIC_SYNTH_H
#define TONIC_SYNTH_H

#include <map>
#include "BufferFiller.h"
#include "ControlParameter.h"
#include "CompressorLimiter.h"
#include "ControlChangeNotifier.h"

namespace Tonic{
  
  // forward declaration
  class Synth;
  
  namespace Tonic_ {
    
    class Synth_ : public BufferFiller_ {
      
    protected:
      
      Generator     outputGen_;
      
      Limiter limiter_;
      bool limitOutput_;
      
      std::map<string, ControlParameter> parameters_;
      std::vector<string> orderedParameterNames_;
      std::map<string, ControlChangeNotifier> controlChangeNotifiers_;
      vector<ControlChangeNotifier> controlChangeNotifiersList_;
      // ControlGenerators that may not be part of the synthesis graph, but should be ticked anyway
      vector<ControlGenerator> auxControlGenerators_;
      
      void computeSynthesisBlock(const Tonic::Tonic_::SynthesisContext_ &context);
      
    public:
      
      Synth_();
      
      //! Set the output gen that produces audio for the Synth
      void  setOutputGen(Generator gen){ outputGen_ = gen; }
      const Generator getOutputGen() { return outputGen_; };
      
      void setLimitOutput(bool shouldLimit) { limitOutput_ = shouldLimit; };
      
      ControlParameter addParameter(string name, TonicFloat initialValue);
      
      void addParameter(ControlParameter parameter);
      
      void addParametersFromSynth(Synth synth);
      
      void setParameter(string name, float value, bool normalized = false);
      
      vector<ControlParameter>  getParameters();
      
      ControlChangeNotifier publishChanges(ControlGenerator input, string name);
      
      void addAuxControlGenerator(ControlGenerator generator){
        auxControlGenerators_.push_back(generator);
      }
      
      void forceNewOutput(){
        synthContext_.forceNewOutput = true;
      }
      
      void sendControlChangesToSubscribers();
      
      void addControlChangeSubscriber(string name, ControlChangeSubscriber* resp);
      void addControlChangeSubscriber(ControlChangeSubscriber* resp);
      void removeControlChangeSubscriber(ControlChangeSubscriber* sub);
      
    };
    
    inline void Synth_::computeSynthesisBlock(const SynthesisContext_ &context){

      outputGen_.tick(outputFrames_, context);
      
      for (vector<ControlGenerator>::iterator it = auxControlGenerators_.begin(); it != auxControlGenerators_.end(); it++) {
        it->tick(context);
      }
      
      if (limitOutput_){
        limiter_.tickThrough(outputFrames_, context);
      }
    }
    
  }
  
  // ---- Smart Pointer -----
  
  class Synth  : public TemplatedBufferFiller<Tonic_::Synth_> {
    
  public:
        
    //! Set the output gen that produces audio for the Synth
    void  setOutputGen(Generator generator){
      gen()->lockMutex();
      gen()->setOutputGen(generator);
      gen()->unlockMutex();
    }
    
    //! Returns a reference to outputGen
    const Generator getOutputGen() {
      return gen()->getOutputGen();
    }

    //! Set whether synth uses dynamic limiter to prevent clipping/wrapping. Defaults to true.
    void setLimitOutput(bool shouldLimit) {
      gen()->setLimitOutput(shouldLimit);
    }
    
    //! Add a ControlParameter with name "name"
    ControlParameter addParameter(string name, TonicFloat initialValue = 0.f)
    {
      return gen()->addParameter(name, initialValue);
    }
    
    //! Add a ControlParameter initialized elsewhere. Will overwrite existing parameter with the same name.
    void addParameter(ControlParameter parameter)
    {
      gen()->addParameter(parameter);
    }
    
    //! Add all the parameters from another synth
    void addParametersFromSynth(Synth synth)
    {
      gen()->addParametersFromSynth(synth);
    }
    
    /*! 
      Returns a ControlConditioner which accepts an input and a ControlChangeSubscriber (supplied by the UI).
      When the input value changes, ControlChangeSubscriber::messageRecieved is called.
      You would typically call this method inside a synth definition if you have a ControlGenerator whose value you want
      to make accessible to the UI thread. You then subscribe to these events by creating a subclass of ControlChangeSubscriber
      and passing a pointer to that object to Synth::addControlChangeSubscriber.
    */
  
    ControlChangeNotifier publishChanges(ControlGenerator input, string name=""){
      return gen()->publishChanges(input, name);
    }
    
    //! Add a ControlGenerator to a list of objects which will be ticked regardless of whether they're part of the synthesis graph or not.
    void addAuxControlGenerator(ControlGenerator generator){
      gen()->lockMutex();
      gen()->addAuxControlGenerator(generator);
      gen()->unlockMutex();
    }
    
    //! Add an object which will be notified when a particular ControlChangeNotifier changes value or is triggered.
    void addControlChangeSubscriber(string name, ControlChangeSubscriber* resp){
      gen()->addControlChangeSubscriber(name, resp);
    }
    
    //! Add an object which will be notified when any ControlChangeNotifier changes value or is triggered.
    void addControlChangeSubscriber(ControlChangeSubscriber* resp){
      gen()->addControlChangeSubscriber(resp);
    }
    
    //! Unsubscribe a ControlChangeSubscriber 
    void removeControlChangeSubscriber(ControlChangeSubscriber* sub){
      gen()->removeControlChangeSubscriber(sub);
    }
    
    /*! 
      Use in conjunction with publishChanges and addControlChangeSubscriber.
      This is designed as a way to get events from the audio thread to the UI thread. 
      sendControlChangesToSubscribers should be called from the UI thread, not the audio thread.
    */
    void sendControlChangesToSubscribers(){
      gen()->sendControlChangesToSubscribers();
    }
    
    //! Set the value of a control parameter on this synth
    /*!
        If normalized is true, value will be mapped to defined range of parameter
     */
    void setParameter(string name, float value = 1.f, bool normalized = false)
    {
      gen()->setParameter(name, value, normalized);
    }
  
    
    //! Get all of the control parameters registered for this synth
    vector<ControlParameter> getParameters()
    {
      return gen()->getParameters();
    }
    
    void forceNewOutput(){
      gen()->lockMutex();
      gen()->forceNewOutput();
      gen()->unlockMutex();
    }
            
  };

  
  // ------------------------------
  //
  // Adding factory/registration for easy wrappers and instantiation
  //
  // http://stackoverflow.com/questions/582331/is-there-a-way-to-instantiate-objects-from-a-string-holding-their-class-name
  //
  // -----------------------------
  
  template<typename T> Synth createSynth() {
    return T();
  }
  
  struct SynthFactory {
    
    typedef std::map<std::string, Synth(*)()> map_type;
    
    static Synth createInstance(std::string const& s) {
      map_type::iterator it = getMap()->find(s);
      if(it == getMap()->end()){
        string synthsList = "";
        
        for(it = getMap()->begin(); it != getMap()->end(); it++){
          synthsList = synthsList + it->first + "\n";
        }
        
        error("Error creating synth. Synth named \"" + s + "\" not found. Existing registered synths are: \n" + synthsList);
        
        // return empty synth
        return Synth();
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

#define TONIC_REGISTER_SYNTH(SynthName)              \
                                              \
class SynthName ## _RegistrationPoint {       \
                                              \
protected:                                    \
  static SourceRegister<SynthName> reg;       \
                                              \
};                                            \
                                                                                \
SourceRegister<SynthName> SynthName ## _RegistrationPoint::reg(#SynthName);     \

#endif
