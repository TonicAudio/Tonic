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
using Tonic::Source;
using Tonic::SourceFactory;
using Tonic::Mixer;

@interface TonicSynthManager ()
{
  SourceFactory sourceFactory;
  Mixer mixer;
}

@property (nonatomic, strong) NSMutableDictionary *sourceDict;

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
    self.sourceDict = [NSMutableDictionary dictionaryWithCapacity:10];
    [self setupNovocaineOutput];
  }
  return self;
}

- (void)setupNovocaineOutput
{
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

- (void)addSourceWithName:(NSString *)synthName forKey:(NSString *)key
{
    if (key){
      Source *newSource = sourceFactory.createInstance(synthName.UTF8String);
      if (newSource){
        mixer.addInput(newSource);
        [self.sourceDict setValue:[NSValue valueWithPointer:newSource] forKey:key];
      }
    }
    else{
      [NSException raise:NSInvalidArgumentException format:@"Argument \"key\" cannot be nil"];
    }
  
}

- (void)removeSourceForKey:(NSString *)key
{
    if (key){
      Source *source = (Source*)[[self.sourceDict objectForKey:key] pointerValue];
      if (source){
        mixer.removeInput(source);
        delete source;
        [self.sourceDict removeObjectForKey:key];
      }
    }
    else{
      [NSException raise:NSInvalidArgumentException format:@"Argument \"key\" cannot be nil"];
    }
  
}

- (Tonic::Source*)sourceForKey:(NSString *)key
{
  if (key){
    return (Tonic::Source*)[[self.sourceDict valueForKey:key] pointerValue];
  }
  else{
    [NSException raise:NSInvalidArgumentException format:@"Argument \"key\" cannot be nil"];
  }
  return NULL;
}

@end
