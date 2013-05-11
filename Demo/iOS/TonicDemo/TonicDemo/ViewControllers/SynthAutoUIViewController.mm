//
//  SynthAutoUIViewController.m
//  TonicDemo
//
//  Created by Nick Donaldson on 5/11/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#import "SynthAutoUIViewController.h"
#import "TonicSynthManager.h"
#include "Tonic.h"

using Tonic::Synth;
using Tonic::SynthFactory;

#define kSynthKey @"DemoSynth"

@interface SynthAutoUIViewController ()

@property (nonatomic, strong) SynthDemoDef *demoDef;
@property (nonatomic, assign) Synth* synth;

@end

@implementation SynthAutoUIViewController

- (id)initWithSynthDemoDef:(SynthDemoDef *)demoDef
{
    self = [super initWithNibName:nil bundle:nil];
    if (self) {
      self.demoDef = demoDef;
      self.synth = [[TonicSynthManager sharedManager] addSynthWithName:demoDef.synthClassName forKey:kSynthKey];
    }
    return self;
}

- (void)dealloc
{
  [[TonicSynthManager sharedManager] removeSynthForKey:kSynthKey];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
}


@end
