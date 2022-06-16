#import "F2Utils.h"

@implementation F2Utils

+ (NSString *)toJsonString:(id)jsonDict {
    NSString *jsonString = nil;
    @try {
        NSError *err;
        NSData *jsonData = [NSJSONSerialization dataWithJSONObject:jsonDict options:0 error:&err];
        jsonString = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
    } @catch(NSException *exception) {
        NSLog(@"Json Invalid, Error %@", [exception description]);
    }
    return jsonString;
}

+ (id)toJsonObject:(NSString *)jsonString defValye:(id)defValue{
    @try {
        NSData *data = [jsonString dataUsingEncoding:NSUTF8StringEncoding];
        return [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];
    } @catch(NSException *exception) {
        NSLog(@"Json Invalid , Error %@", [exception description]);
        return defValue;
    }
}

+ (id)toJsonObject:(NSString *)jsonString {
    return [F2Utils toJsonObject:jsonString defValye:nil];
}

+ (id)toJsonDictionary:(NSString *)jsonString {
    return [F2Utils toJsonObject:jsonString defValye:@{}];
}

+ (id)toJsonArray:(NSString *)jsonString {
    return [F2Utils toJsonObject:jsonString defValye:@[]];
}

+ (NSDictionary *)resetCallbacksFromOld:(NSDictionary *)config host:(F2Chart *)chart {
    NSDictionary *dic = [self resetCallbacksFromOld:config new:[NSMutableDictionary dictionary] host:chart];
    return dic;
}

+ (NSDictionary *)resetCallbacksFromOld:(NSDictionary *)old new:(NSMutableDictionary *)newDic host:(F2Chart *)chart {
    [old enumerateKeysAndObjectsUsingBlock:^(id _Nonnull key, id _Nonnull obj, BOOL *_Nonnull stop) {
        if([obj isKindOfClass:[F2Callback class]]) {
            F2Callback *callbackObj = obj;
            newDic[key] = callbackObj.functionId;
            [chart bindF2CallbackObj:callbackObj];
        } else if([obj isKindOfClass:[NSDictionary class]]) {
            NSMutableDictionary *sub = [NSMutableDictionary dictionary];
            [self resetCallbacksFromOld:obj new:sub host:chart];
            newDic[key] = sub;
        } else if([obj isKindOfClass:NSArray.class]) {
            NSMutableArray *sub = [NSMutableArray array];
            [self resetCallbacksFromOldArray:obj new:sub host:chart];
            newDic[key] = sub;
        } else {
            newDic[key] = obj;
        }
    }];
    return newDic;
}

+ (NSArray *)resetCallbacksFromOldArray:(NSArray *)old new:(NSMutableArray *)newArray host:(F2Chart *)chart {
    [old enumerateObjectsUsingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        if([obj isKindOfClass:[F2Callback class]]) {
            F2Callback *callbackObj = obj;
            [chart bindF2CallbackObj:callbackObj];
            [newArray addObject:callbackObj];
        } else if([obj isKindOfClass:[NSDictionary class]]) {
            NSMutableDictionary *sub = [NSMutableDictionary dictionary];
            [self resetCallbacksFromOld:obj new:sub host:chart];
            [newArray addObject:sub];
        } else if([obj isKindOfClass:NSArray.class]) {
            NSMutableArray *sub = [NSMutableArray array];
            [self resetCallbacksFromOldArray:obj new:sub host:chart];
            [newArray addObject:sub];
        } else {
            [newArray addObject:obj];
        }
    }];
    return newArray;
}

@end
