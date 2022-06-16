#import "F2Chart.h"
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

#define F2SafeString(str) (str.length > 0 ? str : @"")
#define F2SafeJson(json) (json.length > 0 ? json : @"[]")

#define F2WeakSelf __weak __typeof(self) weakSelf = self;
#define F2StrongSelf __strong __typeof(weakSelf) strongSelf = weakSelf;
#define F2SafeBlockRun(block, ...) if(block != nil) block(__VA_ARGS__)

#define F2NativeScale UIScreen.mainScreen.nativeScale

@interface F2Utils : NSObject

/// jsonDict可能是NSDictionary or NSArray
+ (NSString *)toJsonString:(id)jsonDict;


/// 解析成NSArray或者NSDictionary, 如果解析失败返回nil
/// @param jsonString json格式
+ (id)toJsonObject:(NSString *)jsonString;

/// 解析成NSArray或者NSDictionary, 如果解析失败返回空字典
/// @param jsonString json格式
+ (id)toJsonDictionary:(NSString *)jsonString;

/// 解析成NSArray或者NSDictionary, 如果解析失败返回空数组
/// @param jsonString json格式
+ (id)toJsonArray:(NSString *)jsonString;

+ (NSDictionary *)resetCallbacksFromOld:(NSDictionary *)config host:(F2Chart *)chart;

@end

NS_ASSUME_NONNULL_END
