//
//  F2Canvas.h
//  AFWFinanceChart
//
//  Created by weiqing.twq on 2021/7/5.
//  Copyright © 2021 Alipay. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN
@class F2CanvasContext;
@protocol F2CanvasContextProtocol <NSObject>

@required;
/// 创建渲染的canvas，因为AntG是异步创建canvas的，所以这里需要回调
/// 解耦AntG后可以优化下方向的命名..
/// @param frame  canvas的size
/// @param callback 回调canvas
- (void)createContextWithFrame:(CGRect)frame complete:(void (^)(F2CanvasContext *))callback;

/// 渲染到屏幕上
- (void)draw;

/// 释放context
- (void)destroy;

///获取C++的context对象
- (void *)context2d;

/// context所在的view
- (UIView *)contextView;

///截图
- (UIImage *)snapshot;

@end

@interface F2CanvasContext : NSObject <F2CanvasContextProtocol>


@end

NS_ASSUME_NONNULL_END
