//
//  SynthChooserViewController.m
//  TonicDemo
//
//  Created by Nick Donaldson on 2/8/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#import "SynthChooserViewController.h"
#import "SineSumViewController.h"
#import "AMViewController.h"

// Just going to hard-code everything for now based on enum

enum {
  SynthChooserSineSum = 0,
  SynthChooserSineAM,
  
  SynthChooserNumChoices
  
};

@interface SynthChooserViewController ()

@end

@implementation SynthChooserViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {

    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.navigationItem.title = @"Choose a synth";
    // Do any additional setup after loading the view from its nib.
}

- (void)viewWillAppear:(BOOL)animated
{
  [super viewWillAppear:animated];
  [self.navigationController setNavigationBarHidden:NO animated:animated];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)viewDidUnload {
    [self setSynthTableView:nil];
    [super viewDidUnload];
}

#pragma mark - TableView Delegate

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
  return 76;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
  [tableView deselectRowAtIndexPath:indexPath animated:YES];
  
  UIViewController *vcToPush = nil;
  
  switch(indexPath.row){
    case SynthChooserSineSum:
      vcToPush = (UIViewController*)[[SineSumViewController alloc] initWithNibName:nil bundle:nil];
      break;
      
    case SynthChooserSineAM:
      vcToPush = (UIViewController*)[[AMViewController alloc] initWithNibName:nil bundle:nil];
      break;
      
    default:
      break;
      
  }
  
  if (vcToPush){
    [self.navigationController pushViewController:vcToPush animated:YES];
  }
}

#pragma mark - Table View Data source

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
  return SynthChooserNumChoices;
}

- (UITableViewCell*)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
  UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"SynthChoiceCell"];
  if (cell == nil){
    cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:@"SynthChoiceCell"];
    
    cell.textLabel.font = [UIFont fontWithName:@"HelveticaNeue" size:20];
    cell.detailTextLabel.font = [UIFont fontWithName:@"HelveticaNeue-Italic" size:14];
    cell.detailTextLabel.numberOfLines = 0;
    cell.selectionStyle = UITableViewCellSelectionStyleGray;
  }

  // All hard-coded for now
  NSString *synthName = nil;
  NSString *synthDesc = nil;

  switch(indexPath.row){
    case SynthChooserSineSum:
      synthName = @"10-Sine Additive Spread";
      synthDesc = @"10 sinewave oscillators diverging from central frequency";
      break;
      
    case SynthChooserSineAM:
      synthName = @"Basic Sinusoidal AM";
      synthDesc = @"Basic AM synth with sinusoidal carrier and modulator";
      break;
      
    default:
      break;
      
  }

  cell.textLabel.text = synthName;
  cell.detailTextLabel.text = synthDesc;

  return  cell;
}

@end
