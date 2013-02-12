//
//  TonicViewController.m
//  TonicDemo
//
//  Created by Morgan Packard on 1/25/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#import "TonicViewController.h"
#include "Novocaine.h"
#include <math.h>
#include "Tests.h"

@interface TonicViewController ()

- (void)handlePan:(UIPanGestureRecognizer*)pan;

@end

@implementation TonicViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
  
    Tonic::runPerformanceTests();
  
    Novocaine *audioManager = [Novocaine audioManager];
  
    [audioManager setOutputBlock:^(float *audioToPlay, UInt32 numSamples, UInt32 numChannels) {
      //amSynth.fillBufferOfFloats(audioToPlay, numSamples, numChannels);
      sineSumSynth.fillBufferOfFloats(audioToPlay, numSamples, numChannels);
    }];
  
  UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(handlePan:)];
  [self.view addGestureRecognizer:pan];
  
}

- (void)handlePan:(UIPanGestureRecognizer *)pan{
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
      sineSumSynth.setSpread(spread);
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
