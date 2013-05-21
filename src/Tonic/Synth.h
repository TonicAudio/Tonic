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

namespace Tonic{
  
  namespace Tonic_ {
    
    class Synth_ : public BufferFiller_ {
      
    protected:
      
      Generator     outputGen_;
      
      Limiter limiter_;
      bool limitOutput_;
      
      std::map<string, ControlParameter> parameters_;
      std::vector<string> orderedParameterNames_;
      
      void computeSynthesisBlock(const Tonic::Tonic_::SynthesisContext_ &context);
      
    public:
      
      Synth_();
      
      //! Set the output gen that produces audio for the Synth
      void  setOutputGen(Generator gen){ outputGen_ = gen; }
      const Generator getOutputGen() { return outputGen_; };
      
      void setLimitOutput(bool shouldLimit) { limitOutput_ = shouldLimit; };
      
      ControlParameter & addParameter(string name, TonicFloat initialValue);
      
      void                      setParameter(string name, float value);
      vector<ControlParameter>  getParameters();
      
      void forceNewOutput(){
        synthContext_.forceNewOutput = true;
      }
            
    };
    
    inline void Synth_::computeSynthesisBlock(const SynthesisContext_ &context){

      outputGen_.tick(outputFrames_, context);
      
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
    ControlParameter & addParameter(string name, TonicFloat initialValue = 0.f)
    {
      return gen()->addParameter(name, initialValue);
    }
    
    //! Set the value of a control parameter on this synth
    void setParameter(string name, float value=1)
    {
      gen()->setParameter(name, value);
    }
    
    //! Get all of the control parameters registered for this synth
    vector<ControlParameter>  getParameters()
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
