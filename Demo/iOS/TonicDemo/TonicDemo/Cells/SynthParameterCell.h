//
//  SynthParameterCell.h
//  TonicDemo
//
//  Created by Nick D on 5/11/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#import <UIKit/UIKit.h>
#include "Tonic.h"

@protocol SynthParameterCellDelegate <NSObject>

- (void)synthParameterCellChangedValue:(float)value forParameterName:(std::string)parameterName;

@end

@interface SynthParameterCell : UITableViewCell

@property (nonatomic, readonly, weak) UIPanGestureRecognizer *panGesture;
@property (weak, nonatomic) id<SynthParameterCellDelegate> delegate;

- (void)setColor:(UIColor*)color;
- (void)setSynthParameter:(Tonic::Synth::SynthParameter)parameter;

@end
