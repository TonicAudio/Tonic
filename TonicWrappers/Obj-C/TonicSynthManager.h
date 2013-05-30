//
//  TonicSynthManager.h
//  TonicDemo
//
//  Created by Nick Donaldson on 2/8/13.

//

#import <Foundation/Foundation.h>
#include "Tonic.h"

@interface TonicSynthManager : NSObject

+ (TonicSynthManager*)sharedManager;

@property (assign, nonatomic) BOOL inputEnabled;

- (void)startSession;
- (void)endSession;

- (void)addSynth:(Tonic::Synth)synth forKey:(NSString*)key;
- (void)removeSynthForKey:(NSString*)key;

@end
