//
//  MarketMovingBugFix1.m
//  F2Native
//
//  Created by weiqing.twq on 2022/2/23.
//  Copyright © 2022 com.alipay.xgraph. All rights reserved.
//

#import "KLineTrendBugFix1.h"

@implementation KLineTrendBugFix1

-(instancetype)initWithFrame:(CGRect)frame {
    CGRect chartFrame = CGRectMake(0, 0, frame.size.width, 280);
    if (self = [super initWithFrame:chartFrame]) {}
    return self;
}

- (void)chartRender {
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/bugfix/kline_trend" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    NSDictionary *jsonDict = [F2Utils toJsonObject:jsonData];
    FunctionItemCallback callback = ^NSDictionary *_Nonnull(NSString *_Nonnull functionId, NSDictionary *_Nonnull param) {
        return param;
    };
    self.chart.canvas(self.canvasView).padding(15, 10, 15, 0.f).callback(callback);
    self.chart.config(jsonDict);
    self.chart.render();
}

@end
