//
//  SynthTestViewController.h
//  TonicDemo
//
//  Created by Nick Donaldson on 2/8/2013 during a blizzard
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#import "SynthTestViewController.h"

@interface SynthTestViewController ()

@property (nonatomic, strong) NSString *synthName;
@property (nonatomic, strong) NSString *description;
@property (nonatomic, copy)   SynthTestPanAction panAction;

- (void)addSynthIfNecessary;
- (void)handlePan:(UIPanGestureRecognizer*)pan;

@end

@implementation SynthTestViewController

- (id)initWithSynthName:(NSString *)synthName description:(NSString *)description panAction:(SynthTestPanAction)action
{
  self = [super initWithNibName:@"SynthTestViewController" bundle:nil];
  if (self){
    self.synthName = synthName;
    self.description = description;
    self.panAction = action;
  }
  return self;
}

- (void)dealloc
{
  [[TonicSynthManager sharedManager] removeSynthForKey:@"testsynth"];
}

- (void)viewDidLoad
{
  [super viewDidLoad];
  [self addSynthIfNecessary];
  UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(handlePan:)];
  [self.view addGestureRecognizer:pan];
  
  self.navigationItem.title = self.synthName;
  self.descLabel.text = self.description;
}

- (void)addSynthIfNecessary
{
  if (self.synthName != nil){
    Tonic::Synth *synthInstance = [[TonicSynthManager sharedManager] synthForKey:self.synthName];
    if (synthInstance == nil){
      [[TonicSynthManager sharedManager] addSynthWithName:self.synthName forKey:@"testsynth"];
    }
  }
}

- (void)handlePan:(UIPanGestureRecognizer *)pan{
  
  Tonic::Synth *synthInstance = [[TonicSynthManager sharedManager] synthForKey:@"testsynth"];
  if (synthInstance == NULL) return;
  
  switch (pan.state){
      
    case UIGestureRecognizerStateBegan:
    case UIGestureRecognizerStateChanged:
    {
      // normalize touch point
      CGPoint touchPoint = [pan locationInView:self.view];
      touchPoint.x = Tonic::map(touchPoint.x, 0, self.view.bounds.size.width, 0, 1, true);
      touchPoint.y = 1.0f - Tonic::map(touchPoint.y, 0, self.view.bounds.size.height, 0, 1, true);
      if (self.panAction){
        self.panAction(synthInstance, touchPoint);
      }
      
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
