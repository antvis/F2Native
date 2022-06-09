
#import <Foundation/Foundation.h>
NS_ASSUME_NONNULL_BEGIN
typedef NSDictionary *_Nonnull(^ItemCallback)(NSDictionary *_Nonnull param);

typedef NSDictionary *_Nonnull(^FunctionItemCallback)(NSString *_Nonnull functionId, NSDictionary *_Nonnull param);

@interface F2Callback : NSObject

@property (nonatomic, copy, readonly) NSString *functionId;

+ (instancetype)callback:(ItemCallback)block;

- (NSDictionary *)execute:(NSDictionary *)param;

@end

@interface F2RequestAnimationFrameHandle : F2Callback

+ (instancetype)initWithF2Chart:(id)chart canvas:(id)canvasView;

@end

NS_ASSUME_NONNULL_END
