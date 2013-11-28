//
//  SynthParameterCell.h
//  TonicDemo
//
//  Created by Nick D on 5/11/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#import <UIKit/UIKit.h>
#include "Tonic.h"

@interface SynthParameterCell : UITableViewCell

@property (nonatomic, readonly, weak) UIPanGestureRecognizer *panGesture;

+ (CGFloat)height;
- (void)setColor:(UIColor*)color;
- (void)setSynthParameter:(Tonic::ControlParameter)parameter;

@end
