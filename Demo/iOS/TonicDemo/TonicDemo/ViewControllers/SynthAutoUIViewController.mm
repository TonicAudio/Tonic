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

#import <QuartzCore/QuartzCore.h>

using Tonic::Synth;
using Tonic::SynthFactory;

#define kSynthKey @"DemoSynth"

@interface SynthAutoUIViewController ()
{
  Synth *_synth;
  vector<Synth::SynthParameter> _synthParameters;
}

@property (nonatomic, strong) SynthDemoDef *demoDef;

@end

@implementation SynthAutoUIViewController

- (id)initWithSynthDemoDef:(SynthDemoDef *)demoDef
{
    self = [super initWithNibName:nil bundle:nil];
    if (self) {
      self.demoDef = demoDef;
      _synth = [[TonicSynthManager sharedManager] addSynthWithName:demoDef.synthClassName forKey:kSynthKey];
      
      if (_synth){
        _synthParameters = _synth->getParameters();
      }
      
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
  
    self.descLabel.text = self.demoDef.synthInstructions;
  
    // shadow for label
}
#pragma mark - Table Delegate

- (BOOL)tableView:(UITableView *)tableView shouldHighlightRowAtIndexPath:(NSIndexPath *)indexPath
{
  return NO;
}

#pragma mark - Table Data Source

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
  return _synthParameters.size();
}

- (UITableViewCell*)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
  return [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"useme"];
}

@end
