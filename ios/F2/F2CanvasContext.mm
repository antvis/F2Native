//
//  F2Canvas.m
//  AFWFinanceChart
//
//  Created by weiqing.twq on 2021/7/5.
//  Copyright © 2021 Alipay. All rights reserved.
//

#import "F2CanvasContext.h"

@implementation F2CanvasContext

- (void)createContextWithFrame:(CGRect)frame complete:(void (^)(F2CanvasContext *))callback {
    NSCAssert(NO, @"子类实现");
}

/// 渲染到屏幕上
-(BOOL)draw {
    NSCAssert(NO, @"子类实现");
    return NO;
}

/// 释放context
-(void)destroy {
    NSCAssert(NO, @"子类实现");
}

///获取C++的context对象
-(void *)context2d {
    NSCAssert(NO, @"子类实现");
    return nullptr;
}

- (UIView *)contextView {
    NSCAssert(NO, @"子类实现");
    return nil;
}

- (UIImage *)snapshot {
    NSCAssert(NO, @"子类实现");
    return nil;
}
@end


