//
//  SynthAutoUIViewController.h
//  TonicDemo
//
//  Created by Nick Donaldson on 5/11/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "SynthDemoDef.h"

@interface SynthAutoUIViewController : UIViewController <UITableViewDelegate, UITableViewDataSource>

@property (nonatomic, weak) IBOutlet UITableView *controlTableView;

- (id)initWithSynthDemoDef:(SynthDemoDef*)demoDef;

@end