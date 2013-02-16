//
//  FMDroneViewController.m
//  TonicDemo
//
//  Created by Morgan Packard on 2/15/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#import "FMDroneViewController.h"
#import "TonicSynthManager.h"

#define kSynthKey @"FMDrone"

@interface FMDroneViewController ()

- (void)addSynthIfNecessary;
- (void)handlePan:(UIPanGestureRecognizer*)pan;
@end

@implementation FMDroneViewController

- (void)dealloc
{
  [[TonicSynthManager sharedManager] removeSourceForKey:kSynthKey];
}

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
    // Do any additional setup after loading the view from its nib.
    
    self.navigationItem.title = @"FM Drone";
    [self addSynthIfNecessary];
    UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(handlePan:)];
    [self.view addGestureRecognizer:pan];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)addSynthIfNecessary
{
  if ([[TonicSynthManager sharedManager] sourceForKey:kSynthKey] == nil){
    [[TonicSynthManager sharedManager] addSourceWithName:@"FMDroneSynth" forKey:kSynthKey];
  }
}

@end
