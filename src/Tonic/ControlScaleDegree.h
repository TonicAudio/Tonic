//
//  ControlScaleDegree.h
//  Tonic 
//
//  Created by Morgan Packard on 7/1/13.
//
//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__ControlScaleDegree__
#define __Tonic__ControlScaleDegree__

#include "ControlConditioner.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ControlScaleDegree_ : public ControlConditioner_{
      
    protected:
    
    inline void computeOutput(const SynthesisContext_ & context){
      
      static const int NOTES_PER_OCTAVE = 12;
      
      if( input_.tick(context).triggered ){
        
        int scaleDegree = input_.tick(context).value;
        int octave = scaleDegree / (int)scale.size();
        if (scaleDegree < 0)
        {
          octave -= 1;
        }
    
        float baseNumber = scaleDegree % scale.size();
        output_.value = scale.at(baseNumber) + NOTES_PER_OCTAVE * octave;
        output_.triggered = true;
      }else{
        output_.triggered = false;
      }
    }
      
    public:
      ControlScaleDegree_();
      ~ControlScaleDegree_();
      vector<float> scale;
    };
    
  }
  
  /*!
    Takes a scale (expressed in midi notes) and maps index values on to the scale. If the index points
    past the end of the scale, it wraps, but with an octave added.
  */
  
  class ControlScaleDegree : public TemplatedControlConditioner<ControlScaleDegree, Tonic_::ControlScaleDegree_>{
    
  public:
  
    ControlScaleDegree setScale(vector<float> scale){
      gen()->scale = scale;
      return *this;
    }
    
    int getScaleLen(){
      return gen()->scale.size();
    }

  };
}

#endif /* defined(__Tonic__ControlScaleDegree__) */


