//
//  SynthTestViewController.h
//  TonicDemo
//
//  Created by Nick Donaldson on 2/8/2013 during a blizzard

//

#import "SynthTestViewController.h"

@interface SynthTestViewController ()

@property (nonatomic, strong) NSString *synthName;
@property (nonatomic, strong) NSString *description;
@property (nonatomic, copy)   SynthTestPanAction panAction;
@property (nonatomic, copy) SynthTestAccellerometerAction accelAction;
@property (strong, nonatomic) NSOperationQueue* operationQueue;
@property (strong, nonatomic) CMMotionManager* motionManager;
@property (strong, nonatomic) SynthDemoDef* synthDemoDef;

- (void)addSynthIfNecessary;
- (void)handlePan:(UIPanGestureRecognizer*)pan;

@end

@implementation SynthTestViewController

-(id)initWithSynthDemoDef: (SynthDemoDef*) def{
  self = [super initWithNibName:@"SynthTestViewController" bundle:nil];
  if (self){
    self.synthDemoDef = def;
    self.synthName = def.synthClassName;
    self.description = def.synthInstructions;
    self.panAction = def.synthAction;
  }
  return self;
}

- (void)dealloc
{
  [[TonicSynthManager sharedManager] removeSynthForKey:@"testsynth"];
  [[TonicSynthManager sharedManager] setInputEnabled:NO];
}

- (void)viewDidLoad
{
  [super viewDidLoad];
  [self addSynthIfNecessary];
  UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(handlePan:)];
  [self.view addGestureRecognizer:pan];
  
  self.navigationItem.title = self.synthName;
  self.descLabel.text = self.description;
  
  self.motionManager = [[CMMotionManager alloc] init];

   if ([self.motionManager isAccelerometerAvailable] && self.synthDemoDef.accellerometerAction != nil){
     self.operationQueue = [[NSOperationQueue alloc] init];
        __weak typeof(self) wself = self;
     [self.motionManager
      startAccelerometerUpdatesToQueue:self.operationQueue
      withHandler:^(CMAccelerometerData *accelerometerData, NSError *error) {
        Tonic::Synth *synthInstance = [[TonicSynthManager sharedManager] synthForKey:@"testsynth"];
        if (synthInstance == NULL) return;
        if (wself.synthDemoDef && wself.synthDemoDef.accellerometerAction){
          wself.synthDemoDef.accellerometerAction(synthInstance, accelerometerData);
        }
    
      }];
   } else {
    
   }
}

- (void)addSynthIfNecessary
{
  if (self.synthName != nil){
    Tonic::Synth *synthInstance = [[TonicSynthManager sharedManager] synthForKey:self.synthName];
    if (synthInstance == nil){
      if (self.synthDemoDef.usesInput){
        [[TonicSynthManager sharedManager] setInputEnabled:YES];
      }
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

@end
