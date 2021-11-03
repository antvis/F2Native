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

@interface F2Canvas : NSObject

- (void)createCanvascanvasWithFrame:(CGRect)frame
                          andBizId:(NSString *)bizId
                           complete:(void (^)(F2Canvas *))callback;

-(void)draw;

-(void)destroy;

-(void *)context2d;

-(UIView *)canvasView;

-(NSString *)canvasBizId;

///上报监控信息
- (void)logPerformance:(NSTimeInterval)duration withInfo:(NSString *)info;

@end

NS_ASSUME_NONNULL_END
