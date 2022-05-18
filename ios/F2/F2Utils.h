#import "F2Chart.h"
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

#define F2SafeString(str) (str.length > 0 ? str : @"")
#define F2SafeJson(json) (json.length > 0 ? json : @"[]")

#define F2WeakSelf __weak __typeof(self) weakSelf = self;
#define F2StrongSelf __strong __typeof(weakSelf) strongSelf = weakSelf;
#define F2SafeBlockRun(block, ...) block ? block(__VA_ARGS__) : nil

#define F2NativeScale UIScreen.mainScreen.nativeScale

@interface F2Utils : NSObject

/// jsonDict可能是NSDictionary or NSArray
+ (NSString *)toJsonString:(id)jsonDict;

+ (id)toJsonObject:(NSString *)jsonString;

+ (NSDictionary *)resetCallbacksFromOld:(NSDictionary *)config host:(F2Chart *)chart;

@end

NS_ASSUME_NONNULL_END
