//
//  WasmModuleConfig.h
//  warmlib
//
//  Created by tianyang on 2020/3/25.
//  Copyright © 2020年 alipay. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface WasmInstanceConfig : NSObject
@property int stackSize;
@property int heapSize;
@end
