//
//  SynthTestViewController.h
//  TonicDemo
//
//  Created by Nick Donaldson on 2/8/2013 during a blizzard
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "TonicSynthManager.h"
#import <CoreMotion/CoreMotion.h>
#import "SynthDemoDef.h" 

@interface SynthTestViewController : UIViewController

@property (nonatomic, weak) IBOutlet UILabel *descLabel;
@property (strong, nonatomic) CMMotionManager* motionManager;
@property (strong, nonatomic) SynthDemoDef* synthDemoDef;

-(id)initWithSynthDemoDef: (SynthDemoDef*) def;

@end
