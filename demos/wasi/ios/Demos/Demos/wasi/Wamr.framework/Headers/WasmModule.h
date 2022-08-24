//
//  WasmModle.h
//  warmlib
//
//  Created by tianyang on 2020/3/25.
//  Copyright © 2020年 alipay. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "WasmModuleInstance.h"
#import "WasmInstanceConfig.h"

@protocol WasmModule <NSObject>

/// 创建一个 instance 实例
/// @param config  instanceConfig
- (id<WasmModuleInstance>)createInstance:(WasmInstanceConfig*)config;

/// 注册 wasm native api
/// @param name api 名
/// @param className api 类名
- (void)registerApi:(NSString *)name className:(NSString *)className;

/// 注册 wasm native api
/// @param name api 名
/// @param instance api 实例
- (void)registerApi:(NSString *)name instance:(id)instance;

/// 取消注册 api
/// @param name  api 名
- (void)unRegisterApi:(NSString *)name;

/// 销毁, 释放资源
- (void)destroy;

@end
