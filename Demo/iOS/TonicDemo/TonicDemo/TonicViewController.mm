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


@interface TonicViewController ()

@end

@implementation TonicViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
   

  
  Novocaine *audioManager = [Novocaine audioManager];
[audioManager setOutputBlock:^(float *audioToPlay, UInt32 numSamples, UInt32 numChannels) {
    synth.fillBufferOfFloats(audioToPlay, numSamples, numChannels);
}];
  
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
