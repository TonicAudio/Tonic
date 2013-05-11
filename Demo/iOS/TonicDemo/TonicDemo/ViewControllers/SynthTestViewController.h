//
//  SynthTestViewController.h
//  TonicDemo
//
//  Created by Nick Donaldson on 2/8/2013 during a blizzard

//

#import <UIKit/UIKit.h>
#import "TonicSynthManager.h"
#import <CoreMotion/CoreMotion.h>
#import "SynthDemoDef.h" 

@interface SynthTestViewController : UIViewController

@property (nonatomic, weak) IBOutlet UILabel *descLabel;

-(id)initWithSynthDemoDef: (SynthDemoDef*) def;

@end
