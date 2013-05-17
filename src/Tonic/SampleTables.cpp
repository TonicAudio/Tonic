//
//  SampleTables.cpp
//  TonicLib
//
//  Created by Nick Donaldson on 5/17/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

#include "SampleTables.h"
#include <map>

namespace Tonic { namespace Tonic_ {
  
  typedef std::map<std::string, SampleTable> SampleTableMap;
  typedef std::map<std::string, unsigned int> SampleTableRetainCountMap;
 
  static SampleTableMap sampleTables_;
  static SampleTableRetainCountMap sampleTableRetainCounts_;
  
  SampleTable createSampleTable(std::string name, unsigned int length, unsigned int channels)
  {
    SampleTableMap::iterator it = sampleTables_.find(name);
    
    if (it == sampleTables_.end()){
      
      SampleTable newTable = SampleTable(length, channels);
      sampleTables_[name] = newTable;
      retainSampleTable(name);
      return sampleTables_[name];
    }
    
    return it->second;
  }
  
  bool getSampleTable(std::string name, SampleTable * table)
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
  
  // retain count management
  void retainSampleTable(std::string name)
  {
    SampleTableRetainCountMap::iterator it = sampleTableRetainCounts_.find(name);
    
    if (it != sampleTableRetainCounts_.end()){
      it->second += 1;
    }
    else{
      sampleTableRetainCounts_[name] = 1;
    }
  }
  
  void releaseSampleTable(std::string name)
  {
    SampleTableRetainCountMap::iterator it = sampleTableRetainCounts_.find(name);
    if (it != sampleTableRetainCounts_.end()){
      it->second -= 1;
      if (it->second == 0){
        
        sampleTableRetainCounts_.erase(it);
        
        SampleTableMap::iterator fIt = sampleTables_.find(name);
        if (fIt != sampleTables_.end()){
          sampleTables_.erase(fIt);
        }
      }
    }
  }

}}