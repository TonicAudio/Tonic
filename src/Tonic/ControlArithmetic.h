//
//  ControlArithmetic.h
//  Tonic 
//
//  Created by Nick Donaldson on 2/24/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef TONIC_CONTROLARITHMETIC_H
#define TONIC_CONTROLARITHMETIC_H

#include "ControlGenerator.h"
#include "ControlValue.h"

using std::vector;

namespace Tonic {
  
  // -----------------------------------------
  //                 ADDER
  // -----------------------------------------

  namespace Tonic_ {
    
    class ControlAdder_ : public ControlGenerator_{
      
    public:
      
      void input( ControlGenerator input );
      
      ControlGenerator getInput(unsigned int index) { return inputs[index]; };
      unsigned long numInputs() { return inputs.size(); };
      
    protected:
      
      void computeOutput( const SynthesisContext_ & context );
      
      vector<ControlGenerator> inputs;
      
    };
    
    inline void ControlAdder_::computeOutput(const SynthesisContext_ &context){
      
      output_.triggered = false;
      
      for (unsigned int i=0; i<inputs.size(); i++){
        if (inputs[i].tick(context).triggered){
          output_.triggered = true;
          break;
        }
      }
      
      TonicFloat sum = 0.0f;
      for (unsigned int i=0; i<inputs.size(); i++){
        sum += inputs[i].tick(context).value;
      }
      output_.value = sum;
    }
    
  }
  
  class ControlAdder : public TemplatedControlGenerator<Tonic_::ControlAdder_>{
    
  public:
    
    void input( ControlGenerator input ){
      gen()->input( input );
    }
    
    ControlGenerator operator[](unsigned int index){
      return gen()->getInput(index);
    }
    
    unsigned long numInputs(){
      return gen()->numInputs();
    }
  };
  
  static ControlAdder operator+(ControlGenerator a, ControlGenerator b){
    ControlAdder adder;
    adder.input(a);
    adder.input(b);
    return adder;
  }
  
  static ControlAdder operator+(ControlGenerator a, float b){
    ControlAdder adder;
    adder.input(a);
    adder.input(ControlValue(b));
    return adder;
  }
  
  static ControlAdder operator+(float a, ControlGenerator b){
    ControlAdder adder;
    adder.input(ControlValue(a));
    adder.input(ControlGenerator(b));
    return adder;
  }
  
  // -----------------------------------------
  //               SUBTRACTOR
  // -----------------------------------------

  namespace Tonic_ {
    
    class ControlSubtractor_ : public ControlGenerator_{
      
    protected:
      ControlGenerator left;
      ControlGenerator right;
      void computeOutput(const SynthesisContext_ & context);
      
    public:

      void setLeft(ControlGenerator arg){
        left = arg;
      }
      void setRight(ControlGenerator arg){
        right = arg;
      }
    };
    
    inline void ControlSubtractor_::computeOutput(const SynthesisContext_ & context){
      ControlGeneratorOutput leftOut = left.tick(context);
      ControlGeneratorOutput rightOut = right.tick(context);
      if(!leftOut.triggered && !rightOut.triggered){
        output_.triggered = false;
      }else{
        output_.triggered = true;
        output_.value = leftOut.value - rightOut.value;
      }
    }
    
  }
  
  class ControlSubtractor : public TemplatedControlGenerator<Tonic_::ControlSubtractor_>{
    
  public:
    
    TONIC_MAKE_CTRL_GEN_SETTERS(ControlSubtractor, left, setLeft);
    TONIC_MAKE_CTRL_GEN_SETTERS(ControlSubtractor, right, setRight);
    
  };
  
  static ControlSubtractor operator-(ControlGenerator a, ControlGenerator b){
    ControlSubtractor subtractor;
    subtractor.left(a);
    subtractor.right(b);
    return subtractor;
  }
  
  static ControlSubtractor operator-(ControlGenerator a, float b){
    ControlSubtractor subtractor;
    subtractor.left(a);
    subtractor.right(ControlValue(b));
    return subtractor;
  }
  
  static ControlSubtractor operator-(float a, ControlGenerator b){
    ControlSubtractor subtractor;
    subtractor.left(ControlValue(a));
    subtractor.right(ControlGenerator(b));
    return subtractor;
  }
  
  
  // -----------------------------------------
  //               MULTIPLIER
  // -----------------------------------------

  namespace Tonic_ {
    
    class ControlMultiplier_ : public ControlGenerator_{
      
    public:
      
      void input( ControlGenerator input );
      
      ControlGenerator getInput(unsigned int index) { return inputs[index]; };
      unsigned long numInputs() { return inputs.size(); };
      
    protected:
      
      void computeOutput( const SynthesisContext_ & context );
      
      vector<ControlGenerator> inputs;
      
    };
    
    inline void ControlMultiplier_::computeOutput(const SynthesisContext_ &context){
      
      output_.triggered = false;
      
      for (unsigned int i=0; i<inputs.size(); i++){
        if (inputs[i].tick(context).triggered){
          output_.triggered = true;
          break;
        }
      }
      
      TonicFloat product = inputs[0].tick(context).value;
      for (unsigned int i=1; i<inputs.size(); i++){
        product *= inputs[i].tick(context).value;
      }
      output_.value = product;
    }
    
  }
  
  class ControlMultiplier : public TemplatedControlGenerator<Tonic_::ControlMultiplier_>{
    
  public:
    
    void input( ControlGenerator input ){
      gen()->input( input );
    }
    
    ControlGenerator operator[](unsigned int index){
      return gen()->getInput(index);
    }
    
    unsigned long numInputs(){
      return gen()->numInputs();
    }
  };
  
  static ControlMultiplier operator*(ControlGenerator a, ControlGenerator b){
    ControlMultiplier mult;
    mult.input(a);
    mult.input(b);
    return mult;
  }
  
  static ControlMultiplier operator*(ControlGenerator a, float b){
    ControlMultiplier mult;
    mult.input(a);
    mult.input(ControlValue(b));
    return mult;
  }
  
  static ControlMultiplier operator*(float a, ControlGenerator b){
    ControlMultiplier mult;
    mult.input(ControlValue(a));
    mult.input(ControlGenerator(b));
    return mult;
  }
  
  // -----------------------------------------
  //                DIVIDER
  // -----------------------------------------

  namespace Tonic_ {
    
    class ControlDivider_ : public ControlGenerator_{
      
    protected:
      ControlGenerator left;
      ControlGenerator right;
      void computeOutput(const SynthesisContext_ & context);
      
    public:

      void setLeft(ControlGenerator arg){
        left = arg;
      }
      void setRight(ControlGenerator arg){
        right = arg;
      }
      
    };
    
    inline void ControlDivider_::computeOutput(const SynthesisContext_ & context){
      ControlGeneratorOutput leftOut = left.tick(context);
      ControlGeneratorOutput rightOut = right.tick(context);
      
      bool rightIsZero = rightOut.value == 0;
      if(rightIsZero){
        error("ControlGenerator divide by zero encountered. Returning last valid value");
      }
      bool noChange = !leftOut.triggered && !rightOut.triggered;
      if(rightIsZero || noChange){
        output_.triggered = false;
      }else{
        output_.triggered = true;
        output_.value = leftOut.value / rightOut.value;
      }
    }
    
  }
  
  class ControlDivider : public TemplatedControlGenerator<Tonic_::ControlDivider_>{
    
  public:
    
    TONIC_MAKE_CTRL_GEN_SETTERS(ControlDivider, left, setLeft);
    TONIC_MAKE_CTRL_GEN_SETTERS(ControlDivider, right, setRight);
    
  };
  
  static ControlDivider operator/(ControlGenerator a, ControlGenerator b){
    ControlDivider divider;
    divider.left(a);
    divider.right(b);
    return divider;
  }
  
  static ControlDivider operator/(ControlGenerator a, float b){
    ControlDivider divider;
    divider.left(a);
    divider.right(ControlValue(b));
    return divider;
  }
  
  static ControlDivider operator/(float a, ControlGenerator b){
    ControlDivider divider;
    divider.left(ControlValue(a));
    divider.right(ControlGenerator(b));
    return divider;
  }

}

#endif


