//
//  SynthParameterCell.m
//  TonicDemo
//
//  Created by Nick D on 5/11/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#import "SynthParameterCell.h"

#define kCellMargin 10.0f

@interface SynthParameterCell () <UIGestureRecognizerDelegate>
{
  Tonic::Synth::SynthParameter _synthParameter;
}

@property (nonatomic, weak) UILabel *paramNameLabel;
@property (nonatomic, weak) UILabel *paramValueLabel;
@property (nonatomic, weak) UIView *paramSliderView;
@property (nonatomic, readwrite, weak) UIPanGestureRecognizer *panGesture;

- (void)createUI;
- (void)updateValueLabel:(float)value;
- (void)setSliderForNormPosition:(float)normPos;
- (void)handleCellPan:(UIPanGestureRecognizer*)pan;

@end

@implementation SynthParameterCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
      [self createUI];
      
      UIPanGestureRecognizer *panGesture = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(handleCellPan:)];
      [self.contentView addGestureRecognizer:panGesture];
      panGesture.delegate = self;
      self.panGesture = panGesture;
      
    }
    return self;
}

- (void)layoutSubviews
{
  [super layoutSubviews];
  
  CGFloat insetWidth = self.contentView.bounds.size.width - 2*kCellMargin;
  CGRect frame = self.paramNameLabel.frame;
  frame.size.height = [self.paramNameLabel sizeThatFits:CGSizeMake(insetWidth, 100)].height;
  [self.paramNameLabel setFrame:frame];

  frame = self.paramValueLabel.frame;
  frame.origin.y = CGRectGetMaxY(self.paramNameLabel.frame);
  frame.size.height = [self.paramValueLabel sizeThatFits:CGSizeMake(insetWidth, 100)].height;
  [self.paramValueLabel setFrame:frame];
}

- (void)createUI
{
  CGFloat insetWidth = self.contentView.bounds.size.width - 2*kCellMargin;
  
  UIView *sliderView = [[UIView alloc] initWithFrame:self.contentView.bounds];
  sliderView.opaque = YES;
  sliderView.backgroundColor = [UIColor colorWithRed:0.8 green:0.2 blue:0.1 alpha:1.0f];
  sliderView.userInteractionEnabled = NO;
  sliderView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
  [self.contentView addSubview:sliderView];
  self.paramSliderView = sliderView;
  
  UILabel *nameLabel = [[UILabel alloc] initWithFrame:CGRectMake(kCellMargin, kCellMargin, insetWidth, 20)];
  nameLabel.backgroundColor = [UIColor clearColor];
  nameLabel.opaque = NO;
  nameLabel.font = [UIFont fontWithName:@"HelveticaNeue-Medium" size:17.0f];
  nameLabel.textColor = [UIColor blackColor];
  nameLabel.autoresizingMask = UIViewAutoresizingFlexibleBottomMargin | UIViewAutoresizingFlexibleWidth;
  [self.contentView addSubview:nameLabel];
  self.paramNameLabel = nameLabel;
  
  UILabel *valueLabel = [[UILabel alloc] initWithFrame:CGRectMake(kCellMargin, kCellMargin + 20, insetWidth, 20)];
  valueLabel.backgroundColor = [UIColor clearColor];
  valueLabel.opaque = NO;
  valueLabel.font = [UIFont fontWithName:@"HelveticaNeue" size:15.0f];
  valueLabel.shadowOffset = CGSizeMake(0, 1);  valueLabel.autoresizingMask = UIViewAutoresizingFlexibleBottomMargin | UIViewAutoresizingFlexibleWidth;
  [self.contentView addSubview:valueLabel];
  self.paramValueLabel = valueLabel;

}

- (void)handleCellPan:(UIPanGestureRecognizer *)pan
{
  
  if (pan.state == UIGestureRecognizerStateBegan || pan.state == UIGestureRecognizerStateChanged){
  
    // normalized position
    float normPosition = MAX(0, MIN(1.f,[pan locationInView:pan.view].x/pan.view.bounds.size.width));
    normPosition = Tonic::map(normPosition, 0.02f, 0.98f, 0.0f, 1.0f, true); // apply a bit of wiggle room for edges so we can reach min/max
    
    [self setSliderForNormPosition:normPosition];
    
    // set synth parameter
    float paramValue = Tonic::map(normPosition, 0.0f, 1.0f, _synthParameter.min, _synthParameter.max, true);
    [self updateValueLabel:paramValue];
    _synthParameter.value.setValue(paramValue);
  }
}

- (void)setSliderForNormPosition:(float)normPos
{
  // slider UI
  CGRect sliderFrame = self.paramSliderView.frame;
  sliderFrame.size.width = MAX(self.contentView.bounds.size.width * normPos, 2.0f); // leave a tiny bit visible if at 0
  self.paramSliderView.frame = sliderFrame;
}

- (void)updateValueLabel:(float)value
{
  self.paramValueLabel.text = [NSString stringWithFormat:@"%.3f", value];
}

#pragma mark - Property overrides

- (void)setSynthParameter:(Tonic::Synth::SynthParameter)parameter
{
  _synthParameter = parameter;
  self.paramNameLabel.text = [NSString stringWithUTF8String:parameter.displayName.c_str()];

  float paramValue = parameter.value.getValue();
  [self updateValueLabel:paramValue];
  
  float normValue = Tonic::map(paramValue, _synthParameter.min, _synthParameter.max, 0.f, 1.f, true);
  [self setSliderForNormPosition:normValue];
}

- (void)setColor:(UIColor*)color
{
  self.paramSliderView.backgroundColor = color;
}

#pragma mark - Gesture delegate

- (BOOL)gestureRecognizerShouldBegin:(UIGestureRecognizer *)gestureRecognizer
{
  if (gestureRecognizer == self.panGesture){
    CGPoint translation = [(UIPanGestureRecognizer*)gestureRecognizer translationInView:gestureRecognizer.view.superview];
    return (fabs(translation.x) / fabs(translation.y) > 1) ? YES : NO;
  }
  return YES;
}

@end
