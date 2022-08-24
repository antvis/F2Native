//
//  WasmLogProtocol.h
//  warmlib
//
//  Created by the one on 2020/7/1.
//  Copyright © 2020 alipay. All rights reserved.
//

#ifndef WasmLogProtocol_h
#define WasmLogProtocol_h

#import "WasmDefine.h"

@protocol WasmLogProtocol <NSObject>

@required

- (void)nativeLog:(NSString *)log type:(WASMLogType)type;

@end

#endif /* WasmLogProtocol_h */
