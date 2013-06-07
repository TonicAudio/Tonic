//
//  SynthDemoDef.h
//  TonicDemo
//
//  Created by Morgan Packard on 3/28/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreMotion/CoreMotion.h>
#import "Tonic.h"

typedef void (^SynthTestPanAction)(Tonic::Synth synth, CGPoint touchPointNorm);
typedef void (^SynthTestAccellerometerAction)(Tonic::Synth synth, CMAccelerometerData *accelerometerData);

@interface SynthDemoDef : NSObject
  @property (nonatomic, strong) NSString* synthClassName;
  @property (nonatomic, strong) NSString* synthDisplayName;
  @property (nonatomic, strong) NSString* synthDescription;
  @property (nonatomic, strong) NSString* synthInstructions;
  @property BOOL usesInput;
  @property BOOL shouldAutoGenUI; // If TRUE the below blocks are ignored
  @property (nonatomic, copy) SynthTestPanAction synthAction;
  @property (nonatomic, copy) SynthTestAccellerometerAction accellerometerAction;
@end