//
//  SynthAutoUIViewController.m
//  TonicDemo
//
//  Created by Nick Donaldson on 5/11/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#import "SynthAutoUIViewController.h"
#import "TonicSynthManager.h"
#import "SynthParameterCell.h"
#include "Tonic.h"

#import <QuartzCore/QuartzCore.h>

using Tonic::Synth;
using Tonic::SynthFactory;

#define kSynthKey       @"DemoSynth"
#define kCellIdentifier @"ParamCell"

@interface SynthAutoUIViewController ()
{
  Synth *_synth;
  vector<Synth::SynthParameter> _synthParameters;
}

@property (nonatomic, strong) NSArray *cellColors;
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
  
  self.navigationItem.title = self.demoDef.synthDisplayName;
  self.descLabel.text = self.demoDef.synthDescription;
 
  [self.controlTableView registerClass:[SynthParameterCell class] forCellReuseIdentifier:kCellIdentifier];
  
  NSMutableArray *cellColors = [NSMutableArray array];
  [cellColors addObject:[UIColor colorWithRed:1 green:0.349 blue:0 alpha:1]];
  [cellColors addObject:[UIColor colorWithRed:1 green:0.6 blue:0 alpha:1]];
  [cellColors addObject:[UIColor colorWithRed:0.988 green:0 blue:0.192 alpha:1]];
  [cellColors addObject:[UIColor colorWithRed:0 green:0.941 blue:0.655 alpha:1]];
  [cellColors addObject:[UIColor colorWithRed:69.f/255.f green:172.f/255.f blue:1.0f alpha:1.0f]];
  
  self.cellColors = cellColors;
  
  // shadow for label
  CALayer* cl = self.descContainer.layer;
  cl.masksToBounds = NO;
  cl.shadowColor = [UIColor blackColor].CGColor;
  cl.shadowOpacity = 0.4f;
  cl.shadowRadius = 3.0f;
  cl.shadowOffset = CGSizeMake(0, 1);
  cl.shadowPath = [UIBezierPath bezierPathWithRect:cl.bounds].CGPath;
  
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
  Synth::SynthParameter param = _synthParameters[indexPath.row];
  SynthParameterCell *cell = [tableView dequeueReusableCellWithIdentifier:kCellIdentifier forIndexPath:indexPath];

  [cell setSynthParameter:param];
  
  NSInteger colorIndex = indexPath.row % self.cellColors.count;
  [cell setColor:[self.cellColors objectAtIndex:colorIndex]];
  
  return cell;
}


@end
