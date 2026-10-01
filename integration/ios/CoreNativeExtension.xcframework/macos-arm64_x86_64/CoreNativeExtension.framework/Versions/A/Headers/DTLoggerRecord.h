//
//  DTLoggerRecord.h
//  CoreNativeExtension
//

#import <Foundation/Foundation.h>
#import <CoreNativeExtension/DTLogger.h>

@interface DTLoggerRecord : NSObject

@property (nonatomic, copy, readonly) NSString *identifier;
@property (nonatomic, copy, readonly) NSString *tag;
@property (nonatomic, copy, readonly) NSString *message;
@property (nonatomic, assign, readonly) DTLoggerLevel level;
@property (nonatomic, strong, readonly) NSDate *timestamp;
@property (nonatomic, copy, readonly) NSString *threadName;

- (instancetype)initWithIdentifier:(NSString *)identifier
                                tag:(NSString *)tag
                            message:(NSString *)message
                              level:(DTLoggerLevel)level
                          timestamp:(NSDate *)timestamp
                        threadName:(NSString *)threadName;

@end
