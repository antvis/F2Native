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

+ (id)toJsonObject:(NSString *)jsonString {
    id jsonObj = nil;
    @try {
        NSData *data = [jsonString dataUsingEncoding:NSUTF8StringEncoding];
        jsonObj = [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];
    }@catch (NSException *exception) {
        NSLog(@"Json Invalid , Error %@", [exception description]);
    }
    return jsonObj;
}

+ (NSDictionary *)resetCallbacksFromOld:(NSDictionary *)config host:(F2Chart *)chart {
    NSDictionary *dic = [self resetCallbacksFromOld:config new:[NSMutableDictionary dictionary] host:chart];
    return dic;
}

+ (NSDictionary *)resetCallbacksFromOld:(NSDictionary *)old new:(NSMutableDictionary *)newDic host:(F2Chart *)chart {
    [old enumerateKeysAndObjectsUsingBlock:^(id _Nonnull key, id _Nonnull obj, BOOL *_Nonnull stop) {
        if([obj isKindOfClass:[F2CallbackObj class]]) {
            F2CallbackObj *callbackObj = obj;
            newDic[key] = callbackObj.key;
            [chart bindF2CallbackObj:callbackObj];
        } else if([obj isKindOfClass:[NSDictionary class]]) {
            NSMutableDictionary *sub = [NSMutableDictionary dictionary];
            [self resetCallbacksFromOld:obj new:sub host:chart];
            newDic[key] = sub;
        } else {
            newDic[key] = obj;
        }
    }];
    return newDic;
}

@end
