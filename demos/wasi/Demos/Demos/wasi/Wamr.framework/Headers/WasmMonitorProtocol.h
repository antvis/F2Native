//
//  WasmMonitorProtocol.h
//  warmlib
//
//  Created by the one on 2020/7/2.
//  Copyright © 2020 alipay. All rights reserved.
//

#ifndef WasmMonitorProtocol_h
#define WasmMonitorProtocol_h

@protocol WasmMonitorProtocol <NSObject>

@required

- (void)onEvent:(NSString *)type subType:(NSString *)subType message:(NSString *)message options:(NSDictionary *)options;

- (void)onError:(NSString *)type exception:(NSString *)exception options:(NSDictionary *)options;

@end

#endif /* WasmMonitorProtocol_h */
