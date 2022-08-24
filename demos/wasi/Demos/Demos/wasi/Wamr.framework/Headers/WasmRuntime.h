//
//  WasmRuntime.h
//  warmlib
//
//  Created by tianyang on 2020/3/25.
//  Copyright © 2020年 alipay. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "WasmInitConfig.h"
#import "WasmModule.h"
#import "AWResult.h"
#import "WasmModuleConfig.h"

@interface WasmRuntime : NSObject

+(instancetype) getRuntime;
-(void) load:(WasmInitConfig*)config listener:(void (^)(bool success,NSString* errorMsg))listener;
-(AWResult *)load:(WasmInitConfig *)config;
-(id<WasmModule>) createModule:(NSData *)data config:(WasmModuleConfig *)config;
- (id<WasmModule>)createCModule:(NSData *)data config:(WasmModuleConfig *)config;
-(void) destroy;

@end
