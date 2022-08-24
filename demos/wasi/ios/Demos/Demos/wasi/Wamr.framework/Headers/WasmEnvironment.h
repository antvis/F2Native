//
//  WasmEnvironment.h
//  warmlib
//
//  Created by the one on 2020/7/2.
//  Copyright © 2020 alipay. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN
static NSString *sdkVersionCommit = @"15b050a";

@interface WasmEnvironment : NSObject

+ (NSString *)warmSDKVersion;

@end

NS_ASSUME_NONNULL_END
