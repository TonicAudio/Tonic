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
#include <float.h>
#include "Tonic.h"
#include "BufferFiller.h"

namespace Tonic{
  
  class Synth  : public BufferFiller{
    
  public:
    
    typedef enum{
      
      SynthParameterTypeContinuous = 0,
      
      // TODO: Support these for auto-UI
      SynthParameterTypeToggle,
      SynthParameterTypeMomentary
      
    } SynthParameterType;
    
    
    struct SynthParameter{
      string              name;
      string              displayName;
      ControlValue        value;
      SynthParameterType  type;
      float               min;
      float               max;
      bool                isLogarithmic;
      
      SynthParameter();
    };
    
    Synth();
    
    void                   setParameter(string name, float value=1);
    
    vector<SynthParameter> getParameters();
    
  protected:
    
    // ND: I moved these to protected because only subclasses should call them.
    // No reason to make them publicly available since you can't change the signal chain dynamically.
    
    //! Add a continuous parameter with value, min, and max. DisplayName will be equal to name
    ControlValue  & addParameter(string name, float value=0, float min=0, float max=1.f, bool isLogarithmic=false);
    
    //! Add a continuous parameter with a custom display name
    ControlValue  & addParameter(string name, string displayName, float value=0, float min=0, float max=1.f, bool isLogarithmic=false);
    
    //! Add a binary parameter (on/off). Type defaults to Toggle, but can also be set to Momentary
    ControlValue  & addBinaryParameter(string name);
    ControlValue  & addBinaryParameter(string name, string displayName, bool isMomentary = false);

    //! Helper to insert new parameters to map
    ControlValue & createParameter(SynthParameterType type, string name, string displayName, float value, float min, float max, bool isLogarithmic);

    // set to true in constructor to clamp incoming parameters to defined min/max
    bool  clampsParameters_; 
    std::map<string, SynthParameter> parameters_;
    std::vector<string> orderedParameterNames_;
    
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
