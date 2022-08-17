//
//  BasePointUIView.m
//  F2Native
//
//  Created by 青原 on 2021/1/4.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#import "BasePointChart.h"

@implementation BasePointChart

- (void)chartRender {
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_singlePointChart" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.chart.canvas(self.canvasView).padding(20, 10, 20, 0.f);
    self.chart.source([F2Utils toJsonArray:jsonData]);
    self.chart.point().position(@"x*y").size(@"z", @[@(3), @(10)]).fixedShape(@"circle");
    self.chart.animate(YES);
    self.chart.render();
}

@end
