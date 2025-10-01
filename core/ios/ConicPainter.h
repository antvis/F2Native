//
//  ConicPainter.h
//  AFWFinanceChart
//
//  Created by ougu on 2024/11/12.
//  Copyright © 2024 Alipay. All rights reserved.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface ConicPainter : NSObject

- (instancetype)initWithColors:(NSArray<UIColor *> *)colors startAngle:(double)startAngle endAngle:(double)endAngle context:(CGContextRef)context;
- (void)drawInRect:(CGRect)rect;

@end

NS_ASSUME_NONNULL_END
