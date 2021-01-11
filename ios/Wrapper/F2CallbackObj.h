
#import <Foundation/Foundation.h>
NS_ASSUME_NONNULL_BEGIN
typedef NSDictionary *_Nullable(^ItemCallback)(NSString * param);

typedef void * _Nullable (*interface) (void* caller,const char * parameter);

@interface F2CallbackObj : NSObject

@property (nonatomic, strong, readonly) NSString *key;
@property (readonly)interface call;

+(instancetype)initWithCallback:(ItemCallback) block;

-(void *)execute:(const char *) param;

@end
NS_ASSUME_NONNULL_END
