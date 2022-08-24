//
//  WasmConfigProtool.h
//  wamrlib
//
//  Created by the one on 2020/7/23.
//  Copyright © 2020 alipay. All rights reserved.
//

#ifndef WasmConfigProtool_h
#define WasmConfigProtool_h

@protocol WasmConfigProtocol <NSObject>

@required

- (NSString *)configForKey:(NSString *)configKey;

@end


#endif /* WasmConfigProtool_h */
