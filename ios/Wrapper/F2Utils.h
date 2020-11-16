#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

#define XGSafeString(str) (str.length > 0 ? str : @"")
#define XGSafeJson(json) (json.length > 0 ? json : @"[]")

@interface F2Utils : NSObject

+ (NSString *)toJsonString:(NSDictionary *)jsonDict;

@end

NS_ASSUME_NONNULL_END
