//
//  DTAIRExtensionContext.h
//  CoreNativeExtension
//
//  Created by Michael Archbold on 2/10/2026.
//  Copyright © 2026 distriqt. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreNativeExtension/DTContext.h>
#import <CoreNativeExtension/FlashRuntimeExtensions.h>


@interface DTAIRExtensionContext : NSObject<DTContext>

@property FREContext context;
@property NSString* identifier;

-(id) initWithContext: (FREContext) context
        andIdentifier: (NSString*) identifier;

@end

