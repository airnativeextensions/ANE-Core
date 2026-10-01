//
//  DTLogger.h
//  CoreNativeExtension
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, DTLoggerLevel) {
    DTLoggerLevelNone = 0,
	DTLoggerLevelError = 1,
	DTLoggerLevelWarning = 2,
	DTLoggerLevelInfo = 3,
	DTLoggerLevelDebug = 4,
	DTLoggerLevelVerbose = 5
};

@class DTLoggerRecord;

typedef void (^DTLoggerEventHandler)(DTLoggerRecord *record);

@interface DTLogger : NSObject

/// Sets the maximum verbosity level. Defaults to DTLoggerLevelVerbose.
+ (void)setLogLevel:(DTLoggerLevel)level;

/// Returns the current maximum verbosity level.
+ (DTLoggerLevel)logLevel;

/// Adds a handler for emitted log messages and returns a token for removal.
+ (id)addLogHandler:(DTLoggerEventHandler)handler;

/// Removes a previously registered handler.
+ (void)removeLogHandler:(id)token;

/// Writes an info-level formatted message.
+ (void)log:(NSString *)identifier tag:(NSString *)tag message:(NSString *)message, ...;

/// Writes an error-level formatted message.
+ (void)error:(NSString *)identifier tag:(NSString *)tag message:(NSString *)message, ...;

/// Writes a warning-level formatted message.
+ (void)warning:(NSString *)identifier tag:(NSString *)tag message:(NSString *)message, ...;

/// Writes a debug-level formatted message.
+ (void)debug:(NSString *)identifier tag:(NSString *)tag message:(NSString *)message, ...;

/// Writes a verbose-level formatted message.
+ (void)verbose:(NSString *)identifier tag:(NSString *)tag message:(NSString *)message, ...;

@end
