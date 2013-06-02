//
//  ControlArithmetic.h
//  Tonic 
//
//  Created by Nick Donaldson on 2/24/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__ControlArithmetic__
#define __Tonic__ControlArithmetic__

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
      unsigned int numInputs() { return inputs.size(); };
      
    protected:
      
      void computeOutput( const SynthesisContext_ & context );
      
      vector<ControlGenerator> inputs;
      
    };
    
  }
  
  class ControlAdder : public TemplatedControlGenerator<Tonic_::ControlAdder_>{
    
  public:
    
    void input( ControlGenerator input ){
      gen()->input( input );
    }
    
    ControlGenerator operator[](unsigned int index){
      return gen()->getInput(index);
    }
    
    unsigned int numInputs(){
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
    
  }
  
  class ControlSubtractor : public TemplatedControlGenerator<Tonic_::ControlSubtractor_>{
    
  public:
    
    createControlGeneratorSetters(ControlSubtractor, left, setLeft);
    createControlGeneratorSetters(ControlSubtractor, right, setRight);
    
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
      unsigned int numInputs() { return inputs.size(); };
      
    protected:
      
      void computeOutput( const SynthesisContext_ & context );
      
      vector<ControlGenerator> inputs;
      
    };
    
  }
  
  class ControlMultiplier : public TemplatedControlGenerator<Tonic_::ControlMultiplier_>{
    
  public:
    
    void input( ControlGenerator input ){
      gen()->input( input );
    }
    
    ControlGenerator operator[](unsigned int index){
      return gen()->getInput(index);
    }
    
    unsigned int numInputs(){
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
    
  }
  
  class ControlDivider : public TemplatedControlGenerator<Tonic_::ControlDivider_>{
    
  public:
    
    createControlGeneratorSetters(ControlDivider, left, setLeft);
    createControlGeneratorSetters(ControlDivider, right, setRight);
    
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

#endif /* defined(__Tonic__ControlAdder__) */


