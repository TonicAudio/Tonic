//
//  SynthDemoDef.m
//  TonicDemo
//
//  Created by Morgan Packard on 3/28/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#import "SynthDemoDef.h"

@implementation SynthDemoDef
-(id)init{
  self = [super init];
  self.accellerometerAction = ^(Tonic::Synth* synth, CMAccelerometerData *accelerometerData){};
  return self;
}
@end
