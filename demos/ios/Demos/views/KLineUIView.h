//
//  KLineUIView.h
//  F2Native
//
//  Created by XiaLuo on 2021/1/14.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#import <F2/F2.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface KLineUIView : UIView <F2GestureDelegate>

@property (nonatomic, strong, readonly) F2CanvasView *canvasView;
@property (nonatomic, strong, readonly) F2Chart *candleChart;
@property (nonatomic, strong, readonly) F2Chart *subChart;

@end

NS_ASSUME_NONNULL_END
