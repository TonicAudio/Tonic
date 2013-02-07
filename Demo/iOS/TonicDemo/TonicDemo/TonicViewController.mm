//
//  TonicViewController.m
//  TonicDemo
//
//  Created by Morgan Packard on 1/25/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#import "TonicViewController.h"
#include "Novocaine.h"
#include "Generator.h"
#include <math.h>

@interface TonicViewController ()

- (void)handlePan:(UIPanGestureRecognizer*)pan;

@end

@implementation TonicViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
  

    Novocaine *audioManager = [Novocaine audioManager];
  
    [audioManager setOutputBlock:^(float *audioToPlay, UInt32 numSamples, UInt32 numChannels) {
      synth.fillBufferOfFloats(audioToPlay, numSamples, numChannels);
    }];
  
  UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(handlePan:)];
  [self.view addGestureRecognizer:pan];
  
}

- (void)handlePan:(UIPanGestureRecognizer *)pan{
  switch (pan.state){
      
    case UIGestureRecognizerStatePossible:
    case UIGestureRecognizerStateBegan:
    case UIGestureRecognizerStateChanged:
    {
      CGPoint touchPoint = [pan locationInView:self.view];
      
      // arbitrarily chosen midi note numbers (linear pitch)
      TonicFloat car = mtof(Tonic::map(touchPoint.x, 0.0f, self.view.bounds.size.width, 45, 72));
      
      // exponenetial sweep in frequency, 0-400 Hz
      TonicFloat mod = 400.0f / powf(10.0f, Tonic::map(touchPoint.y, 0.0f, self.view.bounds.size.height, 0.0f, 3.0f));
      
      
      synth.setCarrierFreq(car);
      synth.setModFreq(mod);
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
