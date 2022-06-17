//
//  MarketMovingBugFix1.m
//  F2Native
//
//  Created by weiqing.twq on 2022/2/23.
//  Copyright © 2022 com.alipay.xgraph. All rights reserved.
//

#import "MarketMovingBugFix1.h"

@implementation MarketMovingBugFix1

- (void)chartRender {
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/bugfix/market_trend" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    NSDictionary *jsonDict = [F2Utils toJsonObject:jsonData];
    FunctionItemCallback callback = ^NSDictionary *_Nonnull(NSString *_Nonnull functionId, NSDictionary *_Nonnull param) {
        return param;
    };
    self.chart.canvas(self.canvasView).padding(15, 10, 15, 0.f).callback(callback);
    NSTimeInterval start = [[NSDate date] timeIntervalSince1970] * 1000;
    self.chart.config(jsonDict);
    NSTimeInterval end = [[NSDate date] timeIntervalSince1970] * 1000;
    NSLog(@"config parse cost %fms", (end - start));
    self.chart.render();
}

@end
