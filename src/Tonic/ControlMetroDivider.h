//
//  ControlMetroDivider.h
//  Tonic 
//
//  Created by Nick Donaldson on 5/31/13.
//
// See LICENSE.txt for license and usage information.
//


#ifndef TONIC_CONTROLMETRODIVIDER_H
#define TONIC_CONTROLMETRODIVIDER_H

#include "ControlConditioner.h"

namespace Tonic {
  
  namespace Tonic_ {

    class ControlMetroDivider_ : public ControlConditioner_ {
      
    protected:
      
      void computeOutput(const SynthesisContext_ & context);
      
      ControlGenerator divisonsGen_;
      ControlGenerator offsetGen_;
      
      unsigned int offset_;
      unsigned int tickCounter_;
      
    public:

      ControlMetroDivider_();
      
      void setDivisionsGen( ControlGenerator gen ) { divisonsGen_ = gen; }
      void setOffsetGen( ControlGenerator gen ) { offsetGen_ = gen; }
      
    };
    
    inline   void ControlMetroDivider_::computeOutput(const SynthesisContext_ & context){
      
      offset_ = (unsigned int)(offsetGen_.tick(context).value);
      unsigned int divisions = max(1, divisonsGen_.tick(context).value);
      
      output_.triggered = false;
      
      if (input_.tick(context).triggered)
      {
        unsigned int modcount = (tickCounter_++ + offset_) % divisions;
        if (modcount == 0){
          output_.triggered = true;
        }
        
        if (tickCounter_ >= divisions) tickCounter_ = 0;
      }
      
    }
    
  }
  
  class ControlMetroDivider : public TemplatedControlConditioner<ControlMetroDivider, Tonic_::ControlMetroDivider_>
  {
  public:
    
    TONIC_MAKE_CTRL_GEN_SETTERS(ControlMetroDivider, divisions, setDivisionsGen);
    TONIC_MAKE_CTRL_GEN_SETTERS(ControlMetroDivider, offset, setOffsetGen);
    
  };
}

#endif


