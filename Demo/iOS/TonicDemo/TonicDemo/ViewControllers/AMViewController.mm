//
//  AMViewController.m
//  TonicDemo
//
//  Created by Nick Donaldson on 2/8/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#import "AMViewController.h"
#import "TonicSynthManager.h"
#import "SineAMSynth.h"

#define kSynthKey @"AMSynth"

@interface AMViewController ()

- (void)addSynthIfNecessary;
- (void)handlePan:(UIPanGestureRecognizer*)pan;

@end

@implementation AMViewController

- (void)dealloc
{
  [[TonicSynthManager sharedManager] removeSynthForKey:kSynthKey];
}

- (void)viewDidLoad
{
  [super viewDidLoad];
  self.navigationItem.title = @"Sine Sum";
  
  
  [self addSynthIfNecessary];
  
  UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(handlePan:)];
  [self.view addGestureRecognizer:pan];
  
}

- (void)addSynthIfNecessary
{
  SineAMSynth *synthInstance = (SineAMSynth*)[[TonicSynthManager sharedManager] synthForKey:kSynthKey];
  if (synthInstance == nil){
    [[TonicSynthManager sharedManager] addSynthWithName:@"SineAMSynth" forKey:kSynthKey];
  }
}

- (void)handlePan:(UIPanGestureRecognizer *)pan{
  
  SineAMSynth *amSynth = (SineAMSynth*)[[TonicSynthManager sharedManager] synthForKey:kSynthKey];
  
  if (amSynth == NULL) return;
  
  
  switch (pan.state){
      
    case UIGestureRecognizerStateBegan:
    case UIGestureRecognizerStateChanged:
    {
      CGPoint touchPoint = [pan locationInView:self.view];
      
      // arbitrarily chosen midi note numbers (linear pitch)
      TonicFloat car = mtof(Tonic::map(touchPoint.x, 0.0f, self.view.bounds.size.width, 47, 88));

      // exponenetial sweep in frequency, 0-1000 Hz
      TonicFloat mod = 1000.0f / powf(10.0f, Tonic::map(touchPoint.y, 0.0f, self.view.bounds.size.height, 0.0f, 3.0f));

      amSynth->setCarrierFreq(car);
      amSynth->setModFreq(mod);

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
