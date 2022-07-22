//
//  F2CoreGraphicsCanvasContext.h
//  F2
//
//  Created by weiqing.twq on 2021/12/8.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface F2CanvasContext : NSObject

/// 创建渲染的canvas, 内部会创建CGContextRef
/// @param size  canvas的size
- (instancetype)initWithSize:(CGSize)size;

/// 如果画布的大小与之前的不同，则更新画布的大小
/// @param size 画布的大小
- (void)changeSize:(CGSize)size;

///获取C++的context对象
///@return CGContextRef 渲染的上下文
- (CGContextRef)context2d;

/// 画布
/// @return CGImageRef bitmap
- (CGImageRef)bitmap;

/// 返回画布的密度
- (CGFloat)nativeScale;

@end

NS_ASSUME_NONNULL_END
