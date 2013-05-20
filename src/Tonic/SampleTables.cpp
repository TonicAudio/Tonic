//
//  SampleTables.cpp
//  TonicLib
//
//  Created by Nick Donaldson on 5/17/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

#include "SampleTables.h"

namespace Tonic {
  
  namespace Tonic_ {
    
    SampleTable_::SampleTable_(unsigned int frames, unsigned int channels){
      frames_.resize(frames, channels);
    }
    
  }
  
  static SampleTableCollection s_sharedSampleTables_;
  
  SampleTableCollection & SharedSampleTables(){
    return s_sharedSampleTables_;
  }
  
  void SampleTableCollection::registerSampleTable(std::string name, SampleTable table)
  {
    SampleTableMap::iterator it = sampleTables_.find(name);
    
    if (it == sampleTables_.end()){
      sampleTables_[name] = table;
    }
    
  }
  
  bool SampleTableCollection::getSampleTable(std::string name, SampleTable * table)
  {    
    SampleTableMap::iterator it = sampleTables_.find(name);
    if (it != sampleTables_.end()){
      if (table){
        *table = it->second;
      }
      return true;
    }
    return false;
  }
  
  void SampleTableCollection::unregisterSampleTable(std::string name){
    SampleTableMap::iterator it = sampleTables_.find(name);
    if (it != sampleTables_.end()){
      sampleTables_.erase(it);
    }
  }

}