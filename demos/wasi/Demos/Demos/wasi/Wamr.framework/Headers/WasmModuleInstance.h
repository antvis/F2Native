//
//  WasmModuleInstance.h
//  warmlib
//
//  Created by tianyang on 2020/3/25.
//  Copyright © 2020年 alipay. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AWResult.h"

@protocol WasmModuleInstance <NSObject>

-(AWResult *)execute;
-(AWResult *)execute:(NSString*)name sig:(NSString*)sig param:(NSArray*)param;
-(void)destroy;

@end
