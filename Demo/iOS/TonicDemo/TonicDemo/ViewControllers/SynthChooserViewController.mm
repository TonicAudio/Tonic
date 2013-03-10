//
//  SynthChooserViewController.m
//  TonicDemo
//
//  Created by Nick Donaldson on 2/8/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#import "SynthChooserViewController.h"
#import "SynthTestViewController.h"
#import "SineSumSynth.h"
#import "SineAMSynth.h"
#import "FMDroneSynth.h"
#import "FilterTest.h"
#import "FilteredNoiseSynth.h"
#import "LFNoiseTestSynth.h"
#import "ADSRTestSynth.h"
#import "RectWaveTestSynth.h"
#import "FlexToothTestSynth.h"
#import "FlexToothLFOTestSynth.h"
#import "PannerTest.h"
#import "MetroTestSynth.h"

@interface SynthDemoDef : NSObject
  @property NSString* synthClassName;
  @property NSString* synthDisplayName;
  @property NSString* synthDescription;
  @property NSString* synthInstructions;
  @property (nonatomic, copy) SynthTestPanAction synthAction;
@end

@implementation SynthDemoDef
@end

@interface SynthChooserViewController (){
  NSMutableArray* synthDefinitions;
}


@end

@implementation SynthChooserViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
      
      // Add synths here in order to have them displayed in the demo app.
      // Note -- the synth header must be imported somewhere or you'll get mysterious
      // synth not found errors at runtime.
      
      synthDefinitions = [NSMutableArray array];
      
      {
        SynthDemoDef* def = [[SynthDemoDef alloc] init];
        [synthDefinitions addObject:def];
        def.synthClassName = @"SineSumSynth";
        def.synthDisplayName = @"10-Sine Additive Spread";
        def.synthDescription = @"10 sinewave oscillators in fifths";
        def.synthInstructions = @"Swipe up and down to change pitch.";
        def.synthAction = ^(Tonic::Synth* synth, CGPoint touchPointNorm){
          synth->setParameter("pitch", touchPointNorm.y);
        };
      }
      
      {
        SynthDemoDef* def = [[SynthDemoDef alloc] init];
        [synthDefinitions addObject:def];
        def.synthClassName = @"SineAMSynth";
        def.synthDisplayName = @"Basic Sinusoidal AM";
        def.synthDescription = @"Basic AM synth with sinusoidal carrier and modulator";
        def.synthInstructions = @"Swipe up and down to change modulator freq. Swipe L/R to change carrier freq.";
        def.synthAction = ^(Tonic::Synth* synth, CGPoint touchPointNorm){
          // arbitrarily chosen midi note numbers (linear pitch)
          TonicFloat car = Tonic::mtof(Tonic::map(touchPointNorm.x, 0.0f, 1.0f, 47, 88));
          
          // exponenetial sweep in frequency, 0-1000 Hz
          TonicFloat mod = 1000.0f / powf(10.0f, Tonic::map(touchPointNorm.y, 0.0f, 1.0f, 3.0f, 0.0f));

          synth->setParameter("carrierFreq", car);
          synth->setParameter("modFreq", mod);
        };
      }
      
      
      {
        SynthDemoDef* def = [[SynthDemoDef alloc] init];
        [synthDefinitions addObject:def];
        def.synthClassName = @"FMDroneSynth";
        def.synthDisplayName = @"FM Drone";
        def.synthDescription = @"Basic FM synth with sinusoidal carrier and modulator";
        def.synthInstructions = @"Swipe up and down to change modulation amount. Swipe L/R to change modulator and carrier freqs.";
        def.synthAction = ^(Tonic::Synth* synth, CGPoint touchPointNorm){
          synth->setParameter("carrierFreq", touchPointNorm.x * 30);
          synth->setParameter("modIndex", touchPointNorm.y*touchPointNorm.y );
        };
      }
      
      {
        SynthDemoDef* def = [[SynthDemoDef alloc] init];
        [synthDefinitions addObject:def];
        def.synthClassName = @"FilterTest";
        def.synthDisplayName = @"Filter Test";
        def.synthDescription = @"Basic FM synth with sinusoidal carrier and modulator";
        def.synthInstructions = @"Test of filter implementations.";
        def.synthAction = ^(Tonic::Synth* synth, CGPoint touchPointNorm){
          synth->setParameter("cutoff", 120.0f * powf(10.0f, touchPointNorm.x * 2));
          synth->setParameter("LFO", touchPointNorm.y*touchPointNorm.y );
        };
      }
      
      
      
      {
        SynthDemoDef* def = [[SynthDemoDef alloc] init];
        [synthDefinitions addObject:def];
        def.synthClassName = @"FilteredNoiseSynth";
        def.synthDisplayName = @"Filtered Noise";
        def.synthDescription = @"Filtered Noise";
        def.synthInstructions = @"X axis: cutoff\nY axis: Q";
        def.synthAction = ^(Tonic::Synth* synth, CGPoint touchPointNorm){
          synth->setParameter("Q", touchPointNorm.y * 135);
          synth->setParameter("cutoff", touchPointNorm.x);
        };
      }
      
      
      {
        SynthDemoDef* def = [[SynthDemoDef alloc] init];
        [synthDefinitions addObject:def];
        def.synthClassName = @"LFNoiseTestSynth";
        def.synthDisplayName = @"Sine Wave Modulated By LFNoise";
        def.synthDescription = @"Sine Wave Modulated By LFNoise";
        def.synthInstructions = @"Y Axis: Noise Frequency";
        def.synthAction = ^(Tonic::Synth* synth, CGPoint touchPointNorm){
          synth->setParameter("noiseFreq", 0.1 + 1000 * touchPointNorm.y * touchPointNorm.y);
        };
      }
            
      {
        SynthDemoDef* def = [[SynthDemoDef alloc] init];
        [synthDefinitions addObject:def];
        def.synthClassName = @"ADSRTestSynth";
        def.synthDisplayName = @"ADSR";
        def.synthDescription = @"ADSR";
        def.synthInstructions = @"Touch movement in top half of screen is keydown. Touch movement in bottom is keyup.";
        def.synthAction = ^(Tonic::Synth* synth, CGPoint touchPointNorm){
          float val = touchPointNorm.y > 0.5?  touchPointNorm.y : 0;
          synth->setParameter("trigger", val);
        };
      }
      
      {
        SynthDemoDef* def = [[SynthDemoDef alloc] init];
        [synthDefinitions addObject:def];
        def.synthClassName = @"RectWaveTestSynth";
        def.synthDisplayName = @"Rectangular Wave";
        def.synthDescription = @"Aliasing rectangular waveform oscillator";
        def.synthInstructions = @"X-Axis: pulse width\nY-Axis: frequency";
        def.synthAction = ^(Tonic::Synth* synth, CGPoint touchPointNorm){
          synth->setParameter("freq", mtof(touchPointNorm.y * 24 + 48));
          synth->setParameter("pwm", touchPointNorm.x);
        };
      }
      
      {
        SynthDemoDef* def = [[SynthDemoDef alloc] init];
        [synthDefinitions addObject:def];
        def.synthClassName = @"FlexToothTestSynth";
        def.synthDisplayName = @"Flexible Sawtooth Oscillator";
        def.synthDescription = @"Aliasing sawtooth oscillator as audio oscillator";
        def.synthInstructions = @"X-Axis: slope of sawtooth\nY-Axis: frequency";
        def.synthAction = ^(Tonic::Synth* synth, CGPoint touchPointNorm){
          synth->setParameter("freq", mtof(touchPointNorm.y * 24 + 48));
          synth->setParameter("slope", touchPointNorm.x);
        };
      }
      
      {
        SynthDemoDef* def = [[SynthDemoDef alloc] init];
        [synthDefinitions addObject:def];
        def.synthClassName = @"FlexToothLFOTestSynth";
        def.synthDisplayName = @"Flexible Sawtooth LFO demo";
        def.synthDescription = @"Aliasing sawtooth oscillator as morphable LFO";
        def.synthInstructions = @"X-Axis: slope of sawtooth\nY-Axis: frequency";
        def.synthAction = ^(Tonic::Synth* synth, CGPoint touchPointNorm){
          synth->setParameter("freq", touchPointNorm.y * touchPointNorm.y * 20 + 0.1);
          synth->setParameter("slope", touchPointNorm.x);
        };
      }
      
      {
        SynthDemoDef* def = [[SynthDemoDef alloc] init];
        [synthDefinitions addObject:def];
        def.synthClassName = @"PannerTest";
        def.synthDisplayName = @"Panner";
        def.synthDescription = @"Panner";
        def.synthInstructions = @"X-Axis controls pan";
        def.synthAction = ^(Tonic::Synth* synth, CGPoint touchPointNorm){
          synth->setParameter("pan", touchPointNorm.x * 2 - 1);
        };
      }
      
      {
        SynthDemoDef* def = [[SynthDemoDef alloc] init];
        [synthDefinitions addObject:def];
        def.synthClassName = @"MetroTestSynth";
        def.synthDisplayName = @"Metronome to pulse as gate";
        def.synthDescription = @"Metronome fed to a pulse generator driving ADSR * Noise";
        def.synthInstructions = @"Y-Axis controls BPM";
        def.synthAction = ^(Tonic::Synth* synth, CGPoint touchPointNorm){
          synth->setParameter("bpm", touchPointNorm.y * 1500);
        };
      }
      
      
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
  
  NSString *synthName = nil;
  NSString *description = nil;
  SynthTestPanAction action = nil;
  
  if (indexPath.row < [synthDefinitions count]) {
      SynthDemoDef* def = [synthDefinitions objectAtIndex:indexPath.row];
      synthName = def.synthClassName;
      action = def.synthAction;
      description = def.synthInstructions;
  }
  

  if (synthName){
    SynthTestViewController *stVC = [[SynthTestViewController alloc] initWithSynthName:synthName description:description panAction:action];
    [self.navigationController pushViewController:stVC animated:YES];
  }
}

#pragma mark - Table View Data source

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
  return [synthDefinitions count];
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
  
  if (indexPath.row < [synthDefinitions count]) {
      SynthDemoDef* def = [synthDefinitions objectAtIndex:indexPath.row];
      synthName = def.synthDisplayName;
      synthDesc = def.synthDescription;
  }

  cell.textLabel.text = synthName;
  cell.detailTextLabel.text = synthDesc;

  return  cell;
}

@end
