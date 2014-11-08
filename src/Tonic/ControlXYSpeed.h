//
//  ControlXYSpeed.h
//  Tonic 
//
//  Created by Morgan Packard on 4/8/13.
//

//
// See LICENSE.txt for license and usage information.
//

#ifndef TONIC_CONTROLXYSPEED_H
#define TONIC_CONTROLXYSPEED_H

#include "ControlGenerator.h"
#include <list>

namespace Tonic {
  
  namespace Tonic_ {

    class ControlXYSpeed_ : public ControlGenerator_{
      
    protected:
    
      list<float> vals;
      ControlGenerator x;
      ControlGenerator y;
      
      float lastYVal;
      float lastXVal;
    
      void computeOutput(const SynthesisContext_ & context);
      
    public:
      ControlXYSpeed_();
      void setX(ControlGenerator x);
      void setY(ControlGenerator y);
      
    };
    
    inline void ControlXYSpeed_::computeOutput(const SynthesisContext_ & context){
      const int numToAverage = 1;
      ControlGeneratorOutput xOut = x.tick(context);
      ControlGeneratorOutput yOut = y.tick(context);
      if(xOut.triggered || yOut.triggered){
        output_.triggered = true;
        float dX = xOut.value - lastXVal;
        float dY = yOut.value  - lastYVal;
        float speed = sqrt(dX * dX + dY * dY);
        vals.push_back(speed);
        if(vals.size() > numToAverage){
          vals.pop_front();
        }
        float total = 0;
        for(list<float>::iterator it = vals.begin(); it != vals.end(); it++){
          total += *it;
        }
        output_.value = total / vals.size();
        lastXVal = xOut.value;
        lastYVal = yOut.value;
      }else{
        output_.triggered = false;
      }
      
    }
    
  }
  
  /*!
    Given a point described by an x and y coordinate, calculate the speed that the point is moving. This is designed for 
    mouse/finger speed, but could potentially have other uses.
  */
  
  class ControlXYSpeed : public TemplatedControlGenerator<Tonic_::ControlXYSpeed_>{
    
  public:
  
  TONIC_MAKE_CTRL_GEN_SETTERS(ControlXYSpeed, x, setX);
  TONIC_MAKE_CTRL_GEN_SETTERS(ControlXYSpeed, y, setY);

  };
}

#endif


