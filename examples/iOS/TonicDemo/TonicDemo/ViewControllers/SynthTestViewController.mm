//
//  SynthTestViewController.h
//  TonicDemo
//
//  Created by Nick Donaldson on 2/8/2013 during a blizzard

//

#import "SynthTestViewController.h"

using namespace Tonic;

@interface SynthTestViewController ()
{
  Synth _synth;
}

@property (nonatomic, copy)   SynthTestPanAction panAction;
@property (nonatomic, copy) SynthTestAccellerometerAction accelAction;
@property (strong, nonatomic) NSOperationQueue* operationQueue;
@property (strong, nonatomic) CMMotionManager* motionManager;
@property (strong, nonatomic) SynthDemoDef* synthDemoDef;

- (void)handlePan:(UIPanGestureRecognizer*)pan;

@end

@implementation SynthTestViewController

-(id)initWithSynthDemoDef: (SynthDemoDef*) def{
  self = [super initWithNibName:@"SynthTestViewController" bundle:nil];
  if (self){
    self.synthDemoDef = def;
    self.panAction = def.synthAction;
    
    _synth = SynthFactory::createInstance(def.synthClassName.UTF8String);
    [[TonicSynthManager sharedManager] addSynth:_synth forKey:@"testsynth"];
    
    if ( [self respondsToSelector:@selector(setEdgesForExtendedLayout:)] ) {
      self.edgesForExtendedLayout = UIRectEdgeNone;
    }
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
  
  UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(handlePan:)];
  [self.view addGestureRecognizer:pan];
  
  self.navigationItem.title = self.synthDemoDef.synthDisplayName;
  self.descLabel.text = self.synthDemoDef.synthInstructions;
  
  self.motionManager = [[CMMotionManager alloc] init];

   if ([self.motionManager isAccelerometerAvailable] && self.synthDemoDef.accellerometerAction != nil){
     self.operationQueue = [[NSOperationQueue alloc] init];
        __weak typeof(self) wself = self;
     [self.motionManager
      startAccelerometerUpdatesToQueue:self.operationQueue
      withHandler:^(CMAccelerometerData *accelerometerData, NSError *error) {
        if (wself.synthDemoDef && wself.synthDemoDef.accellerometerAction){
          wself.synthDemoDef.accellerometerAction(_synth, accelerometerData);
        }
    
      }];
   }
}


- (void)handlePan:(UIPanGestureRecognizer *)pan{
    
  switch (pan.state){
      
    case UIGestureRecognizerStateBegan:
    case UIGestureRecognizerStateChanged:
    {
      // normalize touch point
      CGPoint touchPoint = [pan locationInView:self.view];
      touchPoint.x = Tonic::map(touchPoint.x, 0, self.view.bounds.size.width, 0, 1, true);
      touchPoint.y = 1.0f - Tonic::map(touchPoint.y, 0, self.view.bounds.size.height, 0, 1, true);
      if (self.panAction){
        self.panAction(_synth, touchPoint);
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
