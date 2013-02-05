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

#define fRand(min, max) ((float)rand()/RAND_MAX)*(max-min)+min

@interface TonicViewController ()

- (void)handleTap;

@end

@implementation TonicViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
  

    Novocaine *audioManager = [Novocaine audioManager];
  
    [audioManager setOutputBlock:^(float *audioToPlay, UInt32 numSamples, UInt32 numChannels) {
      synth.fillBufferOfFloats(audioToPlay, numSamples, numChannels);
    }];
  
  UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleTap)];
  [self.view addGestureRecognizer:tap];
  
}

- (void)handleTap{

  synth.setCarrierFreq(fRand(200, 1200));
  synth.setModFreq(fRand(100, 500));
  
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
