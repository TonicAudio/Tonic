//
//  TonicSynthManager.mm
//  TonicDemo
//
//  Created by Nick Donaldson on 2/8/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#import "TonicSynthManager.h"
#import "Novocaine.h"

#include "Mixer.h"
#include <vector>

using std::vector;
using Tonic::Synth;
using Tonic::SynthFactory;
using Tonic::Mixer;

@interface TonicSynthManager ()
{
  SynthFactory synthFactory;
  Mixer mixer;
}

@property (nonatomic, strong) NSMutableDictionary *synthDict;

- (void)setupNovocaineOutput;

@end

@implementation TonicSynthManager

+ (TonicSynthManager*)sharedManager
{
  static TonicSynthManager *_sharedManager;
  static dispatch_once_t onceToken;
  dispatch_once(&onceToken, ^{
    _sharedManager = [[TonicSynthManager alloc] initInternal];
  });
  return _sharedManager;
}

- (id)init
{
  [NSException raise:NSInternalInconsistencyException format:@"init is not a valid initializer for singleton TonicSynthManager"];
  return nil;
}

- (id)initInternal
{
  self = [super init];
  if (self){
    self.synthDict = [NSMutableDictionary dictionaryWithCapacity:10];
    [self setupNovocaineOutput];
  }
  return self;
}

- (void)setupNovocaineOutput
{
  mixer.setLimitOutput(true);
  
  [[Novocaine audioManager] setOutputBlock:^(float *audioToPlay, UInt32 numSamples, UInt32 numChannels) {
      mixer.fillBufferOfFloats(audioToPlay, numSamples, numChannels);
  }];
  
  [[Novocaine audioManager] pause];
}

- (void)startSession
{
  [[Novocaine audioManager] play];
}

- (void)endSession
{
  [[Novocaine audioManager] pause];
}

- (void)addSynthWithName:(NSString *)synthName forKey:(NSString *)key
{
    if (key){
      Synth *newSynth = synthFactory.createInstance(synthName.UTF8String);
      if (newSynth){
        mixer.addInput(newSynth);
        [self.synthDict setValue:[NSValue valueWithPointer:newSynth] forKey:key];
      }else{
        NSLog(@"Error in TonicSynthManager: Failed to add source. Source named %@ not found.", key);
      }
    }
    else{
      [NSException raise:NSInvalidArgumentException format:@"Argument \"key\" cannot be nil"];
    }
  
}

- (void)removeSynthForKey:(NSString *)key
{
    if (key){
      Synth *synth = (Synth*)[[self.synthDict objectForKey:key] pointerValue];
      if (synth){
        mixer.removeInput(synth);
        delete synth;
        [self.synthDict removeObjectForKey:key];
      }
    }
    else{
      [NSException raise:NSInvalidArgumentException format:@"Argument \"key\" cannot be nil"];
    }
  
}

- (Tonic::Synth*)synthForKey:(NSString *)key
{
  if (key){
    return (Tonic::Synth*)[[self.synthDict valueForKey:key] pointerValue];
  }
  else{
    [NSException raise:NSInvalidArgumentException format:@"Argument \"key\" cannot be nil"];
  }
  return NULL;
}

@end
