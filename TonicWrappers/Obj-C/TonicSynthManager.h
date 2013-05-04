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

- (void)startSession;
- (void)endSession;

- (void)addSynthWithName:(NSString*)sourceName forKey:(NSString*)key;
- (void)removeSynthForKey:(NSString*)key;

- (Tonic::Synth*)synthForKey:(NSString*)key;


@end
