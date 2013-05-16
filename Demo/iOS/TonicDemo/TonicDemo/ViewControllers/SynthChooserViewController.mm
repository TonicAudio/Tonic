//
//  SynthChooserViewController.m
//  TonicDemo
//
//  Created by Nick Donaldson on 2/8/13.

//

#import "SynthChooserViewController.h"
#import "SynthTestViewController.h"
#import "SynthAutoUIViewController.h"
#include "Tonic.h"

using namespace Tonic;

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
      // Note -- add the "registerSynth" macro to your synth file or you'll get mysterious
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
        def.synthClassName = @"FMDroneSynth";
        def.synthDisplayName = @"FM Drone";
        def.synthDescription = @"Basic FM synth with sinusoidal carrier and modulator";
        def.shouldAutoGenUI = YES;
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
        def.synthClassName = @"DelayTestSynth";
        def.synthDisplayName = @"Basic Delay";
        def.synthDescription = @"Popcorn in SPAAAACE";
        def.shouldAutoGenUI = YES;
      }
      
      
      {
        SynthDemoDef* def = [[SynthDemoDef alloc] init];
        [synthDefinitions addObject:def];
        def.synthClassName = @"StereoDelayTestSynth";
        def.synthDisplayName = @"Stereo Delay";
        def.synthDescription = @"Stereo delay effect, mono or stereo input";
        def.synthInstructions = @"";
        def.synthAction = ^(Tonic::Synth* synth, CGPoint touchPointNorm){
          synth->setParameter("freq", touchPointNorm.x * 500);
          synth->setParameter("frequencyRandomAmount", touchPointNorm.y);
          synth->setParameter("decay", touchPointNorm.y * 0.5);
        };
      }
      
      {
        SynthDemoDef* def = [[SynthDemoDef alloc] init];
        [synthDefinitions addObject:def];
        def.synthClassName = @"ControlSnapToScaleTestSynth";
        def.synthDisplayName = @"Octave Snap";
        def.synthDescription = @"Snap notes to a scale";
        def.synthInstructions = @"Y axis is pitch -- sort of . X axis is range.";
        def.synthAction = ^(Tonic::Synth* synth, CGPoint touchPointNorm){
          synth->setParameter("stepperStart", touchPointNorm.y);
          synth->setParameter("stepperSpread", touchPointNorm.x);
//        def.accellerometerAction = ^(Tonic::Synth* synth, CMAccelerometerData *accelerometerData){
//          synth->setParameter("speed", accelerometerData.acceleration.x + 1);
//        };

        };
      }
      
      {
        SynthDemoDef* def = [[SynthDemoDef alloc] init];
        [synthDefinitions addObject:def];
        def.synthClassName = @"CompressorTestSynth";
        def.synthDisplayName = @"Dynamic Compressor";
        def.synthDescription = @"Compress an 808-esque snare";
        def.synthInstructions = @"Y axis is compression threshold. Ratio is 8:1";
        def.shouldAutoGenUI = YES;
      }
      
      {
        SynthDemoDef* def = [[SynthDemoDef alloc] init];
        [synthDefinitions addObject:def];
        def.synthClassName = @"CompressorDuckingTestSynth";
        def.synthDisplayName = @"Ducking Compression";
        def.synthDescription = @"Compress a bass line in time";
        def.synthInstructions = @"Y axis is release time";
        def.synthAction = ^(Tonic::Synth* synth, CGPoint touchPointNorm){
          synth->setParameter("compRelease", Tonic::map(touchPointNorm.y*touchPointNorm.y, 0, 1, 0.025, 0.5, true));
        };
      }
      
      {
        SynthDemoDef* def = [[SynthDemoDef alloc] init];
        [synthDefinitions addObject:def];
        def.synthClassName = @"XYSpeed";
        def.synthDisplayName = @"XY Speed Reactive";
        def.synthDescription = @"Move fast to make it sound cool!";
        def.synthInstructions = @"Move fast to make it sound cool!";
        def.synthAction = ^(Tonic::Synth* synth, CGPoint touchPointNorm){
          synth->setParameter("x", touchPointNorm.x);
          synth->setParameter("y", touchPointNorm.y);
        };
      }
      
      
      {
        SynthDemoDef* def = [[SynthDemoDef alloc] init];
        [synthDefinitions addObject:def];
        def.synthClassName = @"ControlSwitcherTestSynth";
        def.synthDisplayName = @"Control switcher test";
        def.synthDescription = @"Control switcher test";
        def.synthInstructions = @"x and y modulate the parameters of the steps in random ways";
        def.synthAction = ^(Tonic::Synth* synth, CGPoint touchPointNorm){
          synth->setParameter("x", touchPointNorm.x);
          synth->setParameter("y", touchPointNorm.y);
        };
      }
      
      {
        SynthDemoDef* def = [[SynthDemoDef alloc] init];
        [synthDefinitions addObject:def];
        def.synthClassName = @"ReverbTestSynth";
        def.synthDisplayName = @"Reverb";
        def.synthDescription = @"Artificial Reverb";
        def.synthInstructions = @"Yeeeeah";
        def.shouldAutoGenUI = YES;
      }
      
      {
        SynthDemoDef* def = [[SynthDemoDef alloc] init];
        [synthDefinitions addObject:def];
        def.synthClassName = @"SimpleStepSeqSynth";
        def.synthDisplayName = @"Simple Step Sequencer";
        def.synthDescription = @"Simple Step Sequencer";
        def.synthInstructions = @"";
        def.shouldAutoGenUI = YES;
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
  
  if (indexPath.row < [synthDefinitions count]) {
    SynthDemoDef* def = [synthDefinitions objectAtIndex:indexPath.row];
    if (def){
      UIViewController *stVC = def.shouldAutoGenUI ? [[SynthAutoUIViewController alloc] initWithSynthDemoDef:def] : [[SynthTestViewController alloc] initWithSynthDemoDef:def];
      [self.navigationController pushViewController:stVC animated:YES];
    }
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
