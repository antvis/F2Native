//
//  WasmHandlerManager.h
//  warmlib
//
//  Created by the one on 2020/7/2.
//  Copyright © 2020 alipay. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "WasmLogProtocol.h"
#import "WasmMonitorProtocol.h"
#import "WasmConfigProtool.h"

NS_ASSUME_NONNULL_BEGIN

@interface WasmHandlerManager : NSObject

+ (instancetype)sharedInstance;

// log handler
- (void)setLogHandler:(id<WasmLogProtocol>)logHandler;
- (id<WasmLogProtocol>)getLogHandler;

// monitor handler
- (void)setMonitorHandler:(id<WasmMonitorProtocol>)monitorHandler;
- (id<WasmMonitorProtocol>)getMonitorHandler;

// config handler
- (void)setConfigHandler:(id<WasmConfigProtocol>)configHandler;
- (id<WasmConfigProtocol>)configHandler;

@end

NS_ASSUME_NONNULL_END
