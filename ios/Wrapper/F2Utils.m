#import "F2Utils.h"

@implementation F2Utils

+ (NSString *)toJsonString:(NSDictionary *)jsonDict {
    NSString *jsonString = nil;
    @try {
        NSError *err;
        NSData *jsonData = [NSJSONSerialization dataWithJSONObject:jsonDict options:0 error:&err];
        jsonString = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
    } @catch(NSException *exception) {
        NSLog(@"Not Valid Json, Error %@", [exception description]);
    }
    return jsonString;
}

@end
