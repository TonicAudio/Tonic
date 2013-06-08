//
//  Arithmetic.h
//  Tonic
//
//  Created by Morgan Packard on 1/27/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__Adder__
#define __Tonic__Adder__

#include "Generator.h"

namespace Tonic {
  
  // -----------------------------------------
  //                 ADDER
  // -----------------------------------------
  
  namespace Tonic_{
      
    class Adder_ : public Generator_ {
    protected:
      
      std::vector<Generator> inputs;
      TonicFrames workSpace;
      void computeSynthesisBlock( const SynthesisContext_ &context );

    public:
      
      Adder_();
      
      void input(Generator generator);
      
      Generator getInput(unsigned int index) { return inputs[index]; };
      unsigned int numInputs() { return inputs.size(); };
            
    };
    
    inline void Adder_::computeSynthesisBlock( const SynthesisContext_ &context ){
      
      TonicFloat *framesData =  &outputFrames_[0];
      
      memset(framesData, 0, sizeof(TonicFloat) * outputFrames_.size());
      
      for (int j =0; j < inputs.size(); j++) {
        inputs[j].tick(workSpace, context);
        outputFrames_ += workSpace; // add each sample in frames to each sample in workspace
      }
      
    }
	
  }
  
  class Adder : public TemplatedGenerator<Tonic_::Adder_>{
  public:
    
    Adder input(Generator input){
      gen()->input(input);
      return *this;
    }
    
    Generator operator[](unsigned int index){
      return gen()->getInput(index);
    }
    
    unsigned int numInputs(){
      return gen()->numInputs();
    }
    
  };
  
  // add a Generator to another thing
  
  static Adder operator + (Generator a, Generator b){
    Adder add;
    add.input(a);
    add.input(b);
    return add;
  }
  
  
  static Adder operator + (float a, Generator b){
    Adder add;
    add.input(FixedValue(a));
    add.input(b);
    return add;
  }
  
  
  static Adder operator + (Generator a, float b){
    Adder add;
    add.input(a);
    add.input(FixedValue(b));
    return add;
  }
  
  
  // Add a Generatator and a ControlGenerator
  
  static Adder operator + (Generator a, ControlGenerator b){
    return a + FixedValue().setValue(b);
  }
  
  static Adder operator + (ControlGenerator a, Generator b){
    return FixedValue().setValue(a) + b;
  }
  
  // Add an Adder and a ControlGenerator
  
  static Adder operator + (Adder a, ControlGenerator b){
    return a + FixedValue().setValue(b);
  }
  
  static Adder operator + (ControlGenerator a, Adder b){
    return FixedValue().setValue(a) + b;
  }
  
  // -----------------------------------------
  //                SUBTRACTOR
  // -----------------------------------------
  
  namespace Tonic_ {
    
    class Subtractor_ : public Generator_{
      
    protected:
      
      Generator left;
      Generator right;
      TonicFrames workSpace;
      
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
    public:
      
      Subtractor_();
      
      void setLeft(Generator arg){
        left = arg;
      }
      
      void setRight(Generator arg){
        right = arg;
      }
      
    };
    
    inline void Subtractor_::computeSynthesisBlock(const SynthesisContext_ &context){
      left.tick(outputFrames_, context);
      right.tick(workSpace, context);
      outputFrames_ -= workSpace;
    }
    
  }
  
  class Subtractor : public TemplatedGenerator<Tonic_::Subtractor_>{
    
  public:
    
    createGeneratorSetters(Subtractor, left, setLeft);
    createGeneratorSetters(Subtractor, right, setRight);
    
  };
  
  
  static Subtractor operator - (Generator a, Generator b){
    Subtractor sub;
    sub.left(a);
    sub.right(b);
    return sub;
  }
  
  static Subtractor operator - (float a, Generator b){
    Subtractor sub;
    sub.left(FixedValue(a));
    sub.right(b);
    return sub;
  }
  
  static Subtractor operator - (Generator a, float b){
    Subtractor sub;
    sub.left(a);
    sub.right(FixedValue(b));
    return sub;
  }
  
  // Subtract a Generatator and a ControlGenerator
  
  static Subtractor operator - (Generator a, ControlGenerator b){
    return a - FixedValue().setValue(b);
  }
  
  static Subtractor operator - (ControlGenerator a, Generator b){
    return FixedValue().setValue(a) - b;
  }
  
  // -----------------------------------------
  //                MULTIPLIER
  // -----------------------------------------
  
  namespace Tonic_{
    
    class Multiplier_ : public Generator_{
      
    protected:
      TonicFrames workSpace;
      vector<Generator> inputs;
      
      void computeSynthesisBlock( const SynthesisContext_ & context );
      
    public:

      Multiplier_();
      
      void input(Generator generator);
      
      Generator getInput(unsigned int index) { return inputs[index]; };
      unsigned int numInputs() { return inputs.size(); };
    };
    
    
    inline void Multiplier_::computeSynthesisBlock( const SynthesisContext_ & context ){
      
      memset(&outputFrames_[0], 0, sizeof(TonicFloat) * outputFrames_.size());
      
      // for the first generator, store the value in the block
      inputs[0].tick(outputFrames_, context);
      
      // for additional generators, use the workspace stkframes for tick, and multiply it into the frames argument
      for(int i = 1; i < inputs.size(); i++) {
        inputs[i].tick(workSpace, context);
        outputFrames_ *= workSpace;
      }
      
    }
    
  }
  
  class Multiplier : public TemplatedGenerator<Tonic_::Multiplier_>{
  public:
    Multiplier input(Generator inputSource){
      gen()->input(inputSource);
      return *this;
    }
    
    Generator operator[](unsigned int index){
      return gen()->getInput(index);
    }
    
    unsigned int numInputs(){
      return gen()->numInputs();
    }
    
  };
  
  static Multiplier operator*(Generator a, Generator b){
    Multiplier mult;
    mult.input(a);
    mult.input(b);
    return mult;
  }
  
  static Multiplier operator*(float a, Generator b){
    return FixedValue(a) * b;
  }
  
  static Multiplier operator*(Generator a, float b){
    return a * FixedValue(b);
  }
  
  
  // Multiply a Generatator and a ControlGenerator
  
  static Multiplier operator * (Generator a, ControlGenerator b){
    return a * FixedValue().setValue(b);
  }
  
  static Multiplier operator * (ControlGenerator a, Generator b){
    return FixedValue().setValue(a) * b;
  }
  
  // Multiply an Multiplier and a ControlGenerator
  
  static Multiplier operator * (Multiplier a, ControlGenerator b){
    return a * FixedValue().setValue(b);
  }
  
  static Multiplier operator * (ControlGenerator a, Multiplier b){
    return FixedValue().setValue(a) * b;
  }
  
  // -----------------------------------------
  //                DIVIDER
  // -----------------------------------------
  
  namespace Tonic_ {
    
    class Divider_ : public Generator_{
      
    protected:
      Generator left;
      Generator right;
      TonicFrames workSpace;
      
      void computeSynthesisBlock( const SynthesisContext_ &context );
      
    public:

      Divider_();
      
      void setLeft(Generator arg){
        left = arg;
      }
      
      void setRight(Generator arg){
        right = arg;
      }
      
    };
    
    inline void Divider_::computeSynthesisBlock(const SynthesisContext_ &context){
      left.tick(outputFrames_, context);
      right.tick(workSpace, context);
      outputFrames_ /= workSpace;
      
    }
    
  }
  
  class Divider : public TemplatedGenerator<Tonic_::Divider_>{
    
  public:
    createGeneratorSetters(Divider, left, setLeft);
    createGeneratorSetters(Divider, right, setRight);
    
  };
  
  
  
  static Divider operator / (Generator a, Generator b){
    Divider div;
    div.left(a);
    div.right(b);
    return div;
  }
  
  static Divider operator / (float a, Generator b){
    Divider div;
    div.left(FixedValue(a));
    div.right(b);
    return div;
  }
  
  static Divider operator / (Generator a, float b){
    Divider div;
    div.left(a);
    div.right(FixedValue(b));
    return div;
  }
  
  // Subtract a Generatator and a ControlGenerator
  
  static Divider operator / (Generator a, ControlGenerator b){
    return a / FixedValue().setValue(b);
  }
  
  static Divider operator / (ControlGenerator a, Generator b){
    return FixedValue().setValue(a) / b;
  }
  
}


#endif /* defined(__TonicDemo__Adder__) */
