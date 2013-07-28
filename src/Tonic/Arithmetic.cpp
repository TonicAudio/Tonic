//
//  Arithmetic.cpp
//  Tonic
//
//  Created by Morgan Packard on 1/27/13.

//
// See LICENSE.txt for license and usage information.
//


#include "Arithmetic.h"

namespace Tonic{ namespace Tonic_ {

  // -----------------------------------------
  //                 ADDER
  // -----------------------------------------
  
  Adder_::Adder_(){
    workSpace_.resize(kSynthesisBlockSize, 1, 0);
  }
  
  void Adder_::input(Generator generator){
    inputs_.push_back( generator );
    if ( generator.isStereoOutput() && !this->isStereoOutput() ){
      setIsStereoOutput(true);
    }
  }
  
  void Adder_::setIsStereoOutput( bool stereo )
  {
    Generator_::setIsStereoOutput(stereo);
    workSpace_.resize(kSynthesisBlockSize, stereo ? 2 : 1, 0);
  }

  
  // -----------------------------------------
  //                SUBTRACTOR
  // -----------------------------------------
  
  Subtractor_::Subtractor_(){
    workSpace_.resize(kSynthesisBlockSize, 1, 0);
  }
  
  void Subtractor_::setLeft(Generator arg){
    if (arg.isStereoOutput() && !isStereoOutput())
    {
      setIsStereoOutput(true);
    }
    left_ = arg;
  }
  
  void Subtractor_::setRight(Generator arg){
    if (arg.isStereoOutput() && !isStereoOutput())
    {
      setIsStereoOutput(true);
    }
    right_ = arg;
  }
  
  void Subtractor_::setIsStereoOutput( bool stereo )
  {
    Generator_::setIsStereoOutput(stereo);
    workSpace_.resize(kSynthesisBlockSize, stereo ? 2 : 1, 0);
  }

  
  // -----------------------------------------
  //                MULTIPLIER
  // -----------------------------------------
  
  Multiplier_::Multiplier_(){
    workSpace_.resize(kSynthesisBlockSize, 1, 0);
  }
  
  void Multiplier_::input(Generator generator){
    inputs_.push_back(generator);
    if ( generator.isStereoOutput() && !isStereoOutput() ){
      setIsStereoOutput(true);
    }
  }
  
  void Multiplier_::setIsStereoOutput( bool stereo )
  {
    Generator_::setIsStereoOutput(stereo);
    workSpace_.resize(kSynthesisBlockSize, stereo ? 2 : 1, 0);
  }
  
  
  // -----------------------------------------
  //                DIVIDER
  // -----------------------------------------
  
  Divider_::Divider_(){
    workSpace_.resize(kSynthesisBlockSize, 1, 0);
  }
  
  void Divider_::setLeft(Generator arg){
    if (arg.isStereoOutput() && !isStereoOutput())
    {
      setIsStereoOutput(true);
    }
    left_ = arg;
  }
  
  void Divider_::setRight(Generator arg){
    if (arg.isStereoOutput() && !isStereoOutput())
    {
      setIsStereoOutput(true);
    }
    right_ = arg;
  }
  
  void Divider_::setIsStereoOutput( bool stereo )
  {
    Generator_::setIsStereoOutput(stereo);
    workSpace_.resize(kSynthesisBlockSize, stereo ? 2 : 1, 0);
  }
  
}}