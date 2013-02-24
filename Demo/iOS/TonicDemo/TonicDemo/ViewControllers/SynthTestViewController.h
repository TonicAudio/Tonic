//
//  SynthTestViewController.h
//  TonicDemo
//
//  Created by Nick Donaldson on 2/8/2013 during a blizzard
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "TonicSynthManager.h"

typedef void (^SynthTestPanAction)(Tonic::Synth* synth, CGPoint touchPointNorm);

@interface SynthTestViewController : UIViewController

@property (nonatomic, weak) IBOutlet UILabel *descLabel;

- (id)initWithSynthName:(NSString*)synthName description:(NSString*)description panAction:(SynthTestPanAction)action;

@end
