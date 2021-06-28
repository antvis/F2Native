
#import <Foundation/Foundation.h>
NS_ASSUME_NONNULL_BEGIN
typedef id _Nullable (^ItemCallback)(NSString *param);

typedef void *_Nullable (*interface)(void *caller, const char *parameter);

@interface F2CallbackObj : NSObject

@property (nonatomic, strong, readonly) NSString *key;
@property (readonly) interface call;

+ (instancetype)initWithCallback:(ItemCallback)block;

- (void *)execute:(const char *)param;

@end

@interface RequestAnimationFrameHandle : F2CallbackObj

+ (instancetype)initWithF2Chart:(id)chart canvas:(id)canvasView;

@end
NS_ASSUME_NONNULL_END
