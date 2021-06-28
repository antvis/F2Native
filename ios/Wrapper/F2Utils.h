#import "F2Chart.h"
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

#define XGSafeString(str) (str.length > 0 ? str : @"")
#define XGSafeJson(json) (json.length > 0 ? json : @"[]")

#define WeakSelf __weak __typeof(self) weakSelf = self;
#define StrongSelf __strong __typeof(weakSelf) strongSelf = weakSelf;

@interface F2Utils : NSObject

+ (NSString *)toJsonString:(NSDictionary *)jsonDict;

+ (id)toJsonObject:(NSString *)jsonString;

+ (NSDictionary *)resetCallbacksFromOld:(NSDictionary *)config host:(F2Chart *)chart;

@end

NS_ASSUME_NONNULL_END
