//
//  SineSumViewController.m
//  TonicDemo
//
//  Created by Nick Donaldson on 2/8/2013 during a blizzard
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#import "SineSumViewController.h"
#import "TonicSynthManager.h"
#import "SineSumSynth.h"

#define kSynthKey @"SineSum"

@interface SineSumViewController ()

- (void)addSynthIfNecessary;
- (void)handlePan:(UIPanGestureRecognizer*)pan;

@end

@implementation SineSumViewController

- (void)dealloc
{
  [[TonicSynthManager sharedManager] removeSourceForKey:kSynthKey];
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
  SineSumSynth *synthInstance = (SineSumSynth*)[[TonicSynthManager sharedManager] sourceForKey:kSynthKey];
  if (synthInstance == nil){
    [[TonicSynthManager sharedManager] addSourceWithName:@"SineSumSynth" forKey:kSynthKey];
  }
}

- (void)handlePan:(UIPanGestureRecognizer *)pan{
  
  Synth *fmSynth =(Synth*)[[TonicSynthManager sharedManager] sourceForKey:kSynthKey];
  
  if (fmSynth == NULL) return;
  
  
  switch (pan.state){
      
    case UIGestureRecognizerStateBegan:
    case UIGestureRecognizerStateChanged:
    {
      CGPoint touchPoint = [pan locationInView:self.view];
      
      fmSynth->sendMessage("baseFreq", touchPoint.y + 100);
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
