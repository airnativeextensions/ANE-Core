//
//  DTContext.h
//  CoreNativeExtension
//
//  Created by Michael Archbold on 2/10/2026.
//  Copyright © 2026 distriqt. All rights reserved.
//

#ifndef DTContext_h
#define DTContext_h

#import <CoreNativeExtension/DTLogger.h>

#if !TARGET_OS_OSX
#import <UIKit/UIKit.h>
#else
#import <AppKit/AppKit.h>
#endif


@protocol DTContext <NSObject>

/// Writes a formatted message to the extension log with the specified level and tag.
-(void) log: (DTLoggerLevel) level tag:(NSString*) tag message: (NSString*) message, ...;

-(void) slog: (DTLoggerLevel) level tag:(NSString*) tag message: (NSString*) message;


/// Writes an info-level formatted message.
- (void)info:(NSString *)tag message:(NSString *)message, ...;

/// Writes an error-level formatted message.
- (void)error:(NSString *)tag message:(NSString *)message, ...;

/// Writes a warning-level formatted message.
- (void)warning:(NSString *)tag message:(NSString *)message, ...;

/// Writes a debug-level formatted message.
- (void)debug:(NSString *)tag message:(NSString *)message, ...;

/// Writes a verbose-level formatted message.
- (void)verbose:(NSString *)tag message:(NSString *)message, ...;


/// Handles an error with a message and code, dispatching it to the appropriate handler.
-(void) handleError:(NSString*) message code:(int) errorCode;

/// Handles an error with a message, code, and tag, dispatching it to the appropriate handler.
-(void) handleError:(NSString*) message code:(int) errorCode tag: (NSString*) tag;


/// Dispatches a message with a code and data to the appropriate handler.
-(void) dispatch: (NSString*) code data: (NSString*) data;



#if !TARGET_OS_OSX
-(UIViewController*) getViewController;
#else
-(NSViewController*) getViewController;
#endif

@end


#endif /* DTContext_h */
