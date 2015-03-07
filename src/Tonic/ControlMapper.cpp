#include "ControlMapper.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

namespace Tonic{

  namespace Tonic_{

    void ControlMapper_::setValues( vector<TonicFloat> vals)
    {

      if (vals.size() % 2 != 0)
      {
        Tonic::error("ControlMapper_::setValues values must be an even length. The vector needs to be input/output pairs.");
      }

      points_.clear();
      for (int i = 0; i < vals.size(); i += 2)
      {
        ControlMapperPoint point;
        point.x = vals.at(i);
        point.y = vals.at(i + 1);
        points_.push_back(point);
      }

    }

    void ControlMapper_::computeOutput( const SynthesisContext_ & context )
    {

      ControlGeneratorOutput inputOut = input_.tick(context);

      TonicFloat inputVal = inputOut.value;

      if (points_.size() == 0)
      {
        output_.value = inputVal;
      }

      if (inputVal <= points_.front().x )
      {
        output_.value = points_.front().y;
      }else if(inputVal >= points_.back().x){
        output_.value = points_.back().y;
      }else{
        int nextPointIndex = 0;
        while (nextPointIndex < points_.size() && points_.at(nextPointIndex).x <= inputVal)
        {
          nextPointIndex++;
        }
        nextPointIndex = Tonic::clamp(nextPointIndex, 0, points_.size() - 1);
        ControlMapperPoint prevPoint = points_.at(nextPointIndex -1);
        ControlMapperPoint nextPoint = points_.at(nextPointIndex);
        float distBetweenPoints = nextPoint.x - prevPoint.x;
        float distancePastPrev = inputVal -prevPoint.x;
        float percentBetweenPoints = distancePastPrev / distBetweenPoints;
        float val = (prevPoint.y * (1 - percentBetweenPoints)) + (nextPoint.y * percentBetweenPoints);
        output_.value = val;
      }

      output_.triggered = inputOut.triggered;
    }

  }

  ControlMapper::ControlMapper(void)
  {
  }


  ControlMapper::~ControlMapper(void)
  {
  }

  ControlMapper& ControlMapper::setValues( vector<TonicFloat> vals)
  {
    gen()->setValues(vals);
    return static_cast<ControlMapper&>(*this);
  }



  ControlMapper& ControlMapper::setValues( string values)
  {
    vector<string> stringVals;

    std::stringstream ss(values);
    std::string item;
    while (std::getline(ss, item, ',')) {
      stringVals.push_back(item);
    }

    vector<TonicFloat> floatVector;
    for(int i = 0; i < stringVals.size(); i++){
      floatVector.push_back( ::atof(stringVals.at(i).c_str()) );
    }

    return setValues( floatVector );

  }

}