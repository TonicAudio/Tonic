//
//  TonicSynthManager.h
//  TonicDemo
//
//  Created by Nick Donaldson on 2/8/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "Source.h"

@interface TonicSynthManager : NSObject

+ (TonicSynthManager*)sharedManager;

- (void)startSession;
- (void)endSession;

- (void)addSourceWithName:(NSString*)sourceName forKey:(NSString*)key;
- (void)removeSourceForKey:(NSString*)key;

- (Tonic::Source*)sourceForKey:(NSString*)key;


@end
