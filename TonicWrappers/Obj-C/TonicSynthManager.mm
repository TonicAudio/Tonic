//
//  TonicSynthManager.mm
//  TonicDemo
//
//  Created by Nick Donaldson on 2/8/13.

//

#import "TonicSynthManager.h"
#import "Novocaine.h"
#include <vector>

using std::vector;
using namespace Tonic;

@interface TonicSynthManager ()

@property Mixer mixer;
@property RingBuffer inputBuffer;

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
    self.inputBuffer.initialize("input", 8192, 2); // enough and then some
    self.inputEnabled = NO;
    [self setupNovocaineOutput];
  }
  return self;
}

- (void)setupNovocaineOutput
{
  __weak TonicSynthManager *wself = self;

  [[Novocaine audioManager] setOutputBlock:^(float *data, UInt32 numFrames, UInt32 numChannels) {
      wself.mixer.fillBufferOfFloats(data, numFrames, numChannels);
  }];
  
  [[Novocaine audioManager] setInputBlock:^(float *data, UInt32 numFrames, UInt32 numChannels){
    if (wself.inputEnabled){
      wself.inputBuffer.write(data, numFrames * numChannels);
    }
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

- (void)setInputEnabled:(BOOL)inputEnabled
{
  _inputEnabled = inputEnabled;
  if (inputEnabled){
    self.inputBuffer.reset();
  }
}

- (Tonic::Synth*)addSynthWithName:(NSString *)synthName forKey:(NSString *)key
{
  Synth *newSynth = nil;
    if (key){
      newSynth = SynthFactory::createInstance(synthName.UTF8String);
      if (newSynth){
        
        Synth *oldSynth = (Synth*)[[self.synthDict valueForKey:key] pointerValue];
        if (oldSynth){
          self.mixer.removeInput(*oldSynth);
        }
        self.mixer.addInput(*newSynth);
        [self.synthDict setValue:[NSValue valueWithPointer:newSynth] forKey:key];
        
      }else{
        NSLog(@"Error in TonicSynthManager: Failed to add source. Source named %@ not found.", key);
      }
    }
    else{
      [NSException raise:NSInvalidArgumentException format:@"Argument \"key\" cannot be nil"];
    }
  return newSynth;
}

- (void)removeSynthForKey:(NSString *)key
{
    if (key){
      Synth *synth = (Synth*)[[self.synthDict objectForKey:key] pointerValue];
      if (synth){
        self.mixer.removeInput(*synth);
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
