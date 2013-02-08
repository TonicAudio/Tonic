//
//  TonicViewController.m
//  TonicDemo
//
//  Created by Morgan Packard on 1/25/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#import "TonicViewController.h"
#import "TonicSynthManager.h"

#import "SineAMSynth.h"
#import "SineSumSynth.h"

#include <math.h>

#define kSynthKey @"SineSum"

@interface TonicViewController ()

- (void)addSynthIfNecessary;
- (void)handlePan:(UIPanGestureRecognizer*)pan;

@end

@implementation TonicViewController

- (void)dealloc
{
  [[TonicSynthManager sharedManager] removeSynthForKey:kSynthKey];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
  
  [self addSynthIfNecessary];
  
  UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(handlePan:)];
  [self.view addGestureRecognizer:pan];
  
}

- (void)addSynthIfNecessary
{
  SineSumSynth *synthInstance = (SineSumSynth*)[[TonicSynthManager sharedManager] synthForKey:kSynthKey];
  if (synthInstance == nil){
    [[TonicSynthManager sharedManager] addSynthWithName:@"SineSumSynth" forKey:kSynthKey];
  }
}

- (void)handlePan:(UIPanGestureRecognizer *)pan{
  
  SineSumSynth *sineSumSynth = (SineSumSynth*)[[TonicSynthManager sharedManager] synthForKey:kSynthKey];
  
  if (sineSumSynth == NULL) return;
  
  
  switch (pan.state){
      
    case UIGestureRecognizerStateBegan:
    case UIGestureRecognizerStateChanged:
    {
      CGPoint touchPoint = [pan locationInView:self.view];
      
//      // arbitrarily chosen midi note numbers (linear pitch)
//      TonicFloat car = mtof(Tonic::map(touchPoint.x, 0.0f, self.view.bounds.size.width, 47, 88));
//      
//      // exponenetial sweep in frequency, 0-1000 Hz
//      TonicFloat mod = 1000.0f / powf(10.0f, Tonic::map(touchPoint.y, 0.0f, self.view.bounds.size.height, 0.0f, 3.0f));
//      
//      amSynth.setCarrierFreq(car);
//      amSynth.setModFreq(mod);
      
      TonicFloat spread = powf(Tonic::map(touchPoint.y, 0.0f, self.view.bounds.size.height, 1.0f, 0.0f), 2.0f);
      sineSumSynth->setSpread(spread);
    }
      break;
      
    case UIGestureRecognizerStateCancelled:
    case UIGestureRecognizerStateEnded:
    case UIGestureRecognizerStateFailed:
      
      break;
      
    default:
      break;
      
  }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
